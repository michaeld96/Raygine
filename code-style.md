# Raygine Coding Style

This is for myself so I can have a concrete style so when looking back on function names, variable, and what not, I can 
recognize these items more easily. 

## Variable Names
These will always be snake case.    
```cpp
int var_name;
```

## Function Names
These will always be camel case.
```cpp
void addOne(int x, int y)
```

## Class and Struct Names
Classes and structs will always be capitalized.
```cpp
class Player;

typedef struct Rect;
```

## Methods
Methods will always be capitalized.
```cpp
Player.TakeDamage(10);
```

## Internal Class Variables
These variables will always have an underscore in-front of their name.
```cpp
class Player
{
private: 
    int _health;
}
```

## Bracket Notation
This will need some work, but at the moment function and classes have brackets on their own line.
```cpp
void foo()
{

}

class Player
{
private:
    int _health;
}
```
On the other-hand, assigning structs will have brackets on same line and then their own.
```cpp
SDL_Rect r = {
    10,
    10, 
    30,
    40
};
```

### Note
This is my own coding style and trying to refine it everyday to make my coding more understandable to me. If there 
is something that seems absurd I am open to comments. :)