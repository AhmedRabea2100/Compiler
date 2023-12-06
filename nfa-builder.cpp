#include <string>
#include <vector>
#include <stack>
#include "nfa-builder.h"
#include "./GrammarParser/GrammarParser.h"
using namespace std;

NFABuilder::NFABuilder() {}

NFA* NFABuilder::build(Token *token)
{
    stack<NFA*> operands;
    for (auto &regexChar : token->getPostfixRegex())
    {
        if (regexChar->cType == CHAR)
        {
            operands.push(new NFA(regexChar->c));
            inputs.insert(regexChar->c);
        }
        else
        {
            switch (regexChar->c)
            {
            case '|':
            {
                NFA* op2 = operands.top();
                operands.pop();

                NFA* op1 = operands.top();
                operands.pop();

                NFA* result = NFA::uni(op1, op2);
                operands.push(result);
                break;
            }
            case '$':
            {
                NFA* op2 = operands.top();
                operands.pop();

                NFA* op1 = operands.top();
                operands.pop();

                NFA* result = NFA::concat(op1, op2);
                operands.push(result);
                break;
            }
            case '*':
            {
                NFA* op = operands.top();
                operands.pop();

                NFA* result = NFA::star(op);
                operands.push(result);
                break;
            }
            case '+':
            {
                NFA* op = operands.top();
                operands.pop();

                NFA* result = NFA::plus(op);
                operands.push(result);
                break;
            }
            case '-':
            {
                NFA* op2 = operands.top();
                operands.pop();

                NFA* op1 = operands.top();
                operands.pop();

                char c1 = op1->start.get_transitions().at(0).get_input();
                char c2 = op2->start.get_transitions().at(0).get_input();

                string infix = expand(c1, c2);
                vector<RegexChar *> postfix = GrammarParser::infixToPostfix(infix);
                Token t = Token(0, "", postfix);

                NFABuilder builder;
                NFA* result = builder.build(&t);
                operands.push(result);
                break;
            }
            }
        }
    }
    NFA* result = operands.top();
    result->end.set_is_final(true);
    result->end.set_priority(token->getPriority());
    result->end.set_type(token->getType());

    start.add_transition(&result->start);
    return result;
}

NFA NFABuilder::build(vector<Token *> tokens)
{
    for (auto &token : tokens)
    {
        build(token);
    }
    return get_NFA();
}

NFA NFABuilder::get_NFA()
{
    NFA nfa = NFA(start, start);
    return nfa;
}

unordered_set<char> NFABuilder::get_inputs()
{
    return inputs;
}

string NFABuilder::expand(char c1, char c2)
{
    string result;
    for (char c = c1; c <= c2; ++c)
    {
        result += c;
        if (c < c2)
        {
            result += '|';
        }
    }
    return result;
}
