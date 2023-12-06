#pragma once
#define EPSILON ' '

class State; // Forward declaration

class Transition
{
public:
    Transition(State *from, State *to, char input);
    Transition(State *from, State *to);

     State *get_from();
     State *get_to();
     char get_input();

private:
     State *from;
     State *to;
     char input;
};
