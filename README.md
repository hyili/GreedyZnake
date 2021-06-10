GreedyZnake on Terminal GameBoard
===
A group of greedy snake gaming AI on ncurses-based gameboard framework using C++. How greedy are you?
![build_and_execute](build_and_execute.gif)
![game_autoplay](game_autoplay.gif)

### Programming Language
Written in C++(C++11) Ncurses library

GreedyZnake
===
### Build
```
# make
```

### Run
```
# ./output/snake
```

### Autoplay
using -a and specify AI strategy/method using -m option below
```
# ./output/snake -a -e -f 60 -s 100 -m O
```

### Keyboard Control
- Move
    - Up/Down/Left/Right
- Pause
    - p
- Quit
    - q
- Restart(Reset)
    - r

### Rules
- A snake keeps searching for food
- A snake grows up its body each time after having a food
- A snake cannot cross over its body
- A snake will stop moving while meeting walls or body

### Options
- Gameboard support adjustable maxfps(-f {FPS}, Frames Per Second, refresh rate)
- Gameboard support adjustable maxaps(-s {APS}, Action-applied Per Second, speed)
- A snake's move speed(-s {speed}) is adjustable, implement based on APS
- A snake's initial body length(-l {length}) is adjustable
- A snake's initial location(-y {location})(-x {location}) is adjustable
- Fence mode(-e) support, the snake will be blocked by fence around the map
- Auto mode(-a) support, using customizable Bot to autoplay the game
- Strategy selection(-m {strategy})
    - 'A': Pure food-finding(A*) and Wandering.
    - 'a': Pure food-finding(A*).
    - 'T': Food-finding(A*), tail-chasing(A*) and Wandering.
    - 't': Food-finding(A*) and tail-chasing(A*).
    - 'O': Advanced food-finding(A*) and filling(A* extension).
    - 'P': Pure tail-chasing(A*) and Wandering.
    - 'p': Pure tail-chasing(A*).
    - 'F': Food-finding(A*) and filling(A* extension).
    - 'W': Wandering.
    - Others: Basic straight forward
