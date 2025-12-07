#include "pregex.h"
#include <assert.h>
#include <string.h>

// Simple implementation using Thompson's virtual machine algorithm

typedef struct State State;
typedef struct Ptrlist Ptrlist;
typedef struct Frag Frag;

struct State {
    int c; // character to match
    State *out; // next state
    State *out1; // for split states
    int lastlist; // used to avoid duplicates
};

struct Ptrlist {
    State* states[64];
    int n;
};

struct Frag {
    State *start;
    Ptrlist out;
};

static Ptrlist list1(State *outp) {
    Ptrlist l = {
        .states = { outp},
        .n = 1
    };
    return l;
}

static Ptrlist append(Ptrlist *l1, Ptrlist *l2) {
    Ptrlist newlist = {0};

    for (int i = 0; i < l1->n; i++) {
        newlist.states[newlist.n++] = l1->states[i];
    }

    for (int i = 0; i < l2->n; i++) {
        newlist.states[newlist.n++] = l2->states[i];
    }
    return newlist;
}

static void patch(Ptrlist *l, State *s) {
    for (int i = 0; i < l->n; i++) {
        l->states[i]->out = s;
    }
}

static State* postfix_to_nfa(const char *postfix, State *states) {
    Frag stack[100];
    Frag *stackp = stack;
    Frag e1, e2, e;
    int nstates = 0;

    #define push(s) (*stackp++ = s)
    #define pop() (*--stackp)

    for(const char* p = postfix; *p; p++) {
        switch(*p) {
            case '.':
                e2 = pop();
                e1 = pop();
                patch(&e1.out, e2.start);
                e.start = e1.start;
                e.out = e2.out;
                push(e);
                break;
            case '|':
                e2 = pop();
                e1 = pop();
                State *s = &states[nstates++];
                s->c = 0; // split
                s->out = e1.start;
                s->out1 = e2.start;
                e.start = s;
                e.out = append(&e1.out, &e2.out);
                push(e);
                break;
            case '*':
                e1 = pop();
                State *s2 = &states[nstates++];
                s2->c = 0; // split
                s2->out = e1.start;
                s2->out1 = NULL; // to be patched
                patch(&e1.out, s2);
                e.start = s2;
                e.out = list1(s2);
                push(e);
                break;
            default: {
                State *s = &states[nstates++];
                s->c = *p;
                s->out = NULL;
                s->out1 = NULL;
                e.start = s;
                e.out = list1(s);
                push(e);
                break;
            }
        }
    }

    e = pop();
    assert(stackp == stack); // stack should be empty

    patch(&e.out, NULL); // NULL indicates accept state
    return e.start;
}

static void addstate(Ptrlist *l, State *s, int listid) {
        if (s == NULL || s->lastlist == listid) return;
        s->lastlist = listid;
        if (s->c == 0) { // split state
            addstate(l, s->out, listid);
            addstate(l, s->out1, listid);
        } else {
            l->states[l->n++] = s;
        }
    }

static int match(State start, const char *string) {
    Ptrlist clist = {0}, nlist = {0};
    int listid = 0;

    listid++;
    addstate(&clist, &start, listid);

    for (const char *p = string; *p; p++) {
        listid++;
        nlist.n = 0;

        for (int i = 0; i < clist.n; i++) {
            State *s = clist.states[i];
            if (s->c == *p) {
                addstate(&nlist, s, listid);
            }
        }

        clist = nlist;
        memset(&nlist, 0, sizeof(nlist));
    }

    for (int i = 0; i < clist.n; i++) {
        if (clist.states[i]->out == NULL) {
            return 1; // matched
        }
    }
    return 0; // no match
}

void regex_to_postfix(const char *regex, char *postfix) {
    // Transform infix regex to postfix notation with implicit concatenation
    const char *p = regex;
    char *out = postfix;
    char stack[100];
    int sp = 0;
    int last_was_operand = 0;

    while (*p) {
        switch (*p) {
            case '*':
                *out++ = '*';
                last_was_operand = 0;
                break;
            case '|':
                while (sp > 0 && (stack[sp - 1] == '|' || stack[sp - 1] == '.')) {
                    *out++ = stack[--sp];
                }
                stack[sp++] = '|';
                last_was_operand = 0;
                break;
            case '(':
            case ')':
                if (*p == '(' && last_was_operand) {
                    while (sp > 0 && stack[sp - 1] != '|' && stack[sp - 1] != '(') {
                        *out++ = stack[--sp];
                    }
                    stack[sp++] = '.';
                }
                if (*p == '(') {
                    stack[sp++] = '(';
                } else {
                    while (sp > 0 && stack[sp - 1] != '(') {
                        *out++ = stack[--sp];
                    }
                    if (sp > 0) sp--; // pop '('
                }
                last_was_operand = (*p == ')');
                break;
            default:
                if (last_was_operand) {
                    while (sp > 0 && stack[sp - 1] != '|' && stack[sp - 1] != '(') {
                        *out++ = stack[--sp];
                    }
                    stack[sp++] = '.';
                }
                *out++ = *p;
                last_was_operand = 1;
                break;
        }
        p++;
    }

    while (sp > 0) {
        *out++ = stack[--sp];
    }
    *out = '\0';
}

int pregex_match(const char *pattern, const char *string){
    char postfix[256];
    memset(postfix, 0, sizeof(postfix));
    regex_to_postfix(pattern, postfix);

    State states[256];
    memset(states, 0, sizeof(states));
    State *start = postfix_to_nfa(postfix, states);
    return match(*start, string);
}
