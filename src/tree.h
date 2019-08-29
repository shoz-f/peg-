/* Copyright (c) 2007 by Ian Piumarta
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the 'Software'),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software.  Acknowledgement
 * of the use of this Software in supporting documentation would be
 * appreciated but is not required.
 * 
 * THE SOFTWARE IS PROVIDED 'AS IS'.  USE ENTIRELY AT YOUR OWN RISK.
 * 
 * Last edited: 2016-07-22 09:42:48 by piumarta on zora.local
 */

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

enum {
    cUnknown = 0,
    cRule,
    cVariable,
    cName,
    cDot,
    cCharacter,
    cString,
    cClass,
    cAction,
    cInline,
    cPredicate,
    cError,
    cAlternate,
    cSequence,
    cPeekFor,
    cPeekNot,
    cQuery,
    cStar,
    cPlus
};

enum {
  RuleUsed      = 1<<0,
  RuleReached   = 1<<1,
};

//typedef union Node Node;

struct Node {
    Node(int type) :type(type) {}

    int   type;
    Node* next;
};

struct CRule :public Node {
    static std::vector<CRule*> _rules;
    static CRule* rules;

    CRule(char* name);
    static CRule* find(char* name);

    std::string mName;
    Node *variables;
    Node *expression;
    int id;
    int flags;
};

CRule* CRule::rules     = NULL;

struct CVariable  :public Node {
    CVariable() :Node(cVariable) {}
    char *name;  Node *value;  int offset;                                 };
struct CName      :public Node {
    CName() :Node(cName) {}
    Node *rule;  Node *variable;                                             };
struct CDot       :public Node {
    CDot() :Node(cDot) {}                                                   };
struct CCharacter :public Node {
    CCharacter() :Node(cCharacter) {}
    char *value;                                                           };
struct CString    :public Node {
    CString() :Node(cString) {}
    char *value;                                                           };
struct CClass     :public Node {
    CClass() :Node(cClass) {}
    unsigned char *value;                                                  };
struct CAction    :public Node {
    CAction() :Node(cAction) {}
    char *text;      Node *list;  char *name;  Node *rule;  int line;              };
struct CInline    :public Node {
    CInline() :Node(cInline) {}
    char *text;                                                                    };
struct CPredicate :public Node {
    CPredicate() :Node(cPredicate) {}
    char *text;                                                                    };
struct CError     :public Node {
    CError() :Node(cError) {}
    Node *element;  char *text;                                                    };
struct CAlternate :public Node {
    CAlternate() :Node(cAlternate) {}
    Node *first;  Node *last;                                                      };
struct CSequence  :public Node {
    CSequence() :Node(cSequence) {}
    Node *first;  Node *last;                                                      };
struct CPeekFor   :public Node {
    CPeekFor() :Node(cPeekFor) {}
    Node *element;                                                         };
struct CPeekNot   :public Node {
    CPeekNot() :Node(cPeekNot) {}
    Node *element;                                                         };
struct CQuery     :public Node {
    CQuery() :Node(cQuery) {}
    Node *element;                                                         };
struct CStar      :public Node {
    CStar() :Node(cStar) {}
    Node *element;                                                             };
struct CPlus      :public Node {
    CPlus() :Node(cPlus) {}
    Node *element;                                                             };
struct CAny       :public Node {                                                            };

/*
union Node
{
  int                   type;
  struct Rule           rule;
  struct Variable       variable;         
  struct Name           name;
  struct Dot            dot;
  struct Character      character;
  struct String         string;
  struct Class          cclass;
  struct Action         action;
  struct Inline         inLine;
  struct Predicate      predicate;
  struct Error          error;
  struct Alternate      alternate;
  struct Sequence       sequence;
  struct PeekFor        peekFor;
  struct PeekNot        peekNot;
  struct Query          query;
  struct Star           star;
  struct Plus           plus;
  struct Any            any;
};
*/

extern Node *actions;
extern CRule *rules;
extern Node *start;

extern FILE *output;

extern CRule *makeRule(char *name);
extern CRule *findRule(char *name);
extern Node *beginRule(Node *rule);
extern void  Rule_setExpression(Node *rule, Node *expression);
extern Node *Rule_beToken(Node *rule);
extern Node *makeVariable(char *name);
extern Node *makeName(Node *rule);
extern Node *makeDot(void);
extern Node *makeCharacter(char *text);
extern Node *makeString(char *text);
extern Node *makeClass(char *text);
extern Node *makeAction(int lineNumber, char *text);
extern Node *makeInline(char *text);
extern Node *makePredicate(char *text);
extern Node *makeError(Node *e, char *text);
extern Node *makeAlternate(Node *e);
extern Node *Alternate_append(Node *e, Node *f);
extern Node *makeSequence(Node *e);
extern Node *Sequence_append(Node *e, Node *f);
extern Node *makePeekFor(Node *e);
extern Node *makePeekNot(Node *e);
extern Node *makeQuery(Node *e);
extern Node *makeStar(Node *e);
extern Node *makePlus(Node *e);
extern Node *push(Node *node);
extern Node *top(void);
extern Node *pop(void);

extern void  Rule_compile_c_header(void);
extern void  Rule_compile_c(Node *node, int nolines);

extern void  Node_print(Node *node);
extern void  Rule_print(Node *node);
