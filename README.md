# Tank Battle

**Tank Battle** is a local two-player tank combat game written in C++ with raylib.

The goal of this project is to build a complete real-time game from scratch: a fixed-timestep simulation, a layered `data / logic / handlers` architecture, collision resolution, and a real search-based AI opponent.

> Tank Battle is currently under active development.

---

## Features

### Currently available

* Local two-player battles: Human vs Human, or Human vs Computer
* Each player commands two tanks, with a key to switch the active tank
* Independent left/right track control per tank (forward, backward, or stay on each side), so movement and rotation come from the same mechanic
* Turret fire with a per-tank cooldown
* A tank's cannon can be shot off independently of destroying the tank body
* Destructible walls with two-stage damage (strong, then weak, then destroyed)
* Mines with a visibility mechanic — a mine goes invisible when a cannon or an active shell passes near it, and reappears otherwise
* Full collision system: shell vs tank body, shell vs cannon, shell vs shell, shell vs wall, mine vs tank
* Win/tie detection when all of a player's tanks are destroyed
* Pause menu (resume, or quit to the main menu)
* Main menu, opponent-selection screen, and an in-game controls/key-bindings screen
* A difficulty choice at the start of a Human vs Computer game: **Easy**, a rule-based opponent that fires on a clear, friendly-fire-free line of sight, evades incoming shells, and otherwise moves semi-randomly; or **Hard**, an alpha-beta pruned minimax search AI (see [SearchAgentPlayer](#searchagentplayer-hard-difficulty) below)
* Fixed-timestep game loop (simulation ticks independently of the render/input frame rate)
* 2D rendering via raylib, with a live HUD panel showing each player's remaining lives
* Per-player configurable key bindings

### Planned

* Improved visuals
* A choice of how many tanks each player plays with (currently fixed at two)

---

## SearchAgentPlayer (Hard difficulty)

`SearchAgentPlayer` (`logic/search_agent_player.h` / `.cpp`) is a `Player` sibling to `ComputerPlayer`, selected by choosing "Hard" at the difficulty prompt. It plays under the same restriction as a human: only its current active tank moves or fires each tick, and it decides for itself when to switch.

* **Active-tank switching** — every tick, `quickSelfEvaluate()` scores both of its own tanks against the nearest live enemy. It only switches to the inactive tank when that tank's score beats the active one's by more than `SEARCH_AGENT_SWITCH_MARGIN`, so it doesn't flip-flop over a marginal difference.
* **Search** — to avoid simulating all four tanks at once, it clones a disposable 1-vs-1 slice of the board (`SimState`, built by `buildSimState`/`synthesizeWallsFromBoard`) for its active tank against the nearest alive enemy, then runs minimax with alpha-beta pruning (`search`, `stepState`) over `SEARCH_AGENT_SEARCH_ROUNDS` rounds of "my action → the enemy's worst-case reply", via `chooseBestAction`. A full replan only runs every `SEARCH_AGENT_REPLAN_TICKS` ticks (~1s) to stay cheap relative to the fixed `GAME_TICK_SECONDS` tick.
* **Evaluation** — leaf positions are scored heuristically (`evaluate`): a clear line of fire (`hasLineOfFire`) is worth more to the AI when the *enemy* has it (`SEARCH_AGENT_LINE_OF_FIRE_DEFENSE_WEIGHT`) than when the AI does (`..._OFFENSE_WEIGHT`), an active shell's projected impact (`projectShellHits`) is weighted by how soon it lands, and adjacent walls/mines add or subtract via `SEARCH_AGENT_WALL_COVER_WEIGHT` / `SEARCH_AGENT_MINE_PROXIMITY_WEIGHT`. This biases the AI toward breaking stand-offs and dodging rather than trading shots evenly.
* **Per-tick safety net** — independent of the replan cadence, every tick it checks whether an active shell is already projected to hit its tank; if so, `chooseSafeAction` immediately picks the best surviving move against that shell, overriding the cached plan. A free, unthreatened shot is likewise taken the instant it's available rather than waiting for the next replan.
* **Friendly-fire prevention** — `wouldHitFriendly()` strips `FIRE` from the candidate actions at the root of `chooseBestAction` whenever it would hit the AI's own other tank, since that tank isn't modeled inside the 1-vs-1 `SimState` at all.

---

## Architecture

The diagram below shows both the **current** architecture and the features planned for the future.

```mermaid
flowchart TD
    User["User"]

    Main["main (tankBattle.cpp)<br/>CURRENT"]
    GameController["GameController<br/>CURRENT"]

    MenuScreens["MenuScreens<br/>CURRENT"]
    DifficultyChoice["Difficulty Choice<br/>CURRENT"]

    HumanPlayer["HumanPlayer<br/>CURRENT"]
    ComputerPlayerEasy["ComputerPlayer (rule-based)<br/>CURRENT / Easy"]
    SearchAgentPlayerHard["SearchAgentPlayer (minimax)<br/>CURRENT / Hard"]

    Board["Board<br/>CURRENT"]
    Tank["Tank<br/>CURRENT"]
    Shell["Shell<br/>CURRENT"]
    Wall["Wall<br/>CURRENT"]
    Mine["Mine<br/>CURRENT"]

    TankMovement["tank_movement<br/>CURRENT"]
    ShellMovement["shell_movement<br/>CURRENT"]
    WallLogic["wall_logic<br/>CURRENT"]
    CollisionRules["collision_rules<br/>CURRENT"]
    InputProvider["IInputProvider<br/>CURRENT"]

    RaylibContext["RaylibContext<br/>CURRENT"]
    RaylibInput["RaylibInputProvider<br/>CURRENT"]
    SceneRenderer["SceneRenderer<br/>CURRENT"]
    HudRenderer["HudRenderer<br/>CURRENT"]
    ImprovedVisuals["Improved Visuals<br/>PLANNED"]
    TankCount["Configurable Tank Count<br/>PLANNED"]

    User --> Main
    Main --> GameController

    GameController --> MenuScreens
    MenuScreens --> DifficultyChoice
    DifficultyChoice --> SearchAgentPlayerHard

    GameController --> HumanPlayer
    GameController --> ComputerPlayerEasy
    GameController --> SearchAgentPlayerHard

    HumanPlayer --> InputProvider
    InputProvider --> RaylibInput

    HumanPlayer --> Tank
    ComputerPlayerEasy --> Tank
    SearchAgentPlayerHard --> Tank
    Tank -.-> TankCount

    GameController --> Board
    GameController --> Shell
    GameController --> Wall
    GameController --> Mine

    GameController --> TankMovement
    GameController --> ShellMovement
    GameController --> WallLogic
    GameController --> CollisionRules

    TankMovement --> Board
    ShellMovement --> Board
    WallLogic --> Board
    CollisionRules --> Board

    GameController --> RaylibContext
    GameController --> SceneRenderer
    GameController --> HudRenderer
    SceneRenderer -.-> ImprovedVisuals
```

**CURRENT** = already implemented

**PLANNED** = planned for a future version

---

## Project Structure

```text
tank_battle/
│
├── constants/
│   └── constants.h        ← board geometry, symbols, tuning, key bindings
│
├── data/                  ← plain game state
│   ├── point.h / .cpp
│   ├── board.h / .cpp
│   ├── tank.h / .cpp
│   ├── shell.h / .cpp
│   ├── wall.h / .cpp
│   └── mine.h / .cpp
│
├── logic/                 ← rules, platform-agnostic
│   ├── player.h / .cpp
│   ├── human_player.h / .cpp
│   ├── computer_player.h / .cpp
│   ├── search_agent_player.h / .cpp
│   ├── input_provider.h
│   ├── tank_spawn.h / .cpp
│   ├── tank_movement.h / .cpp
│   ├── shell_movement.h / .cpp
│   ├── wall_logic.h / .cpp
│   ├── collision_rules.h / .cpp
│   └── char_utils.h
│
├── handlers/               ← raylib-facing orchestration and rendering
│   ├── game_controller.h / .cpp
│   ├── raylib_context.h / .cpp
│   ├── raylib_input_provider.h / .cpp
│   ├── scene_renderer.h / .cpp
│   ├── hud_renderer.h / .cpp
│   └── menu_screens.h / .cpp
│
├── tankBattle.cpp
├── CMakeLists.txt
└── README.md
```

---

## Game Loop & Layering

Tank Battle runs a **fixed-timestep simulation** decoupled from rendering. The game logic advances exactly once per tick regardless of frame rate, while rendering and input polling run every real frame:

```cpp
constexpr double GAME_TICK_SECONDS = 0.5;
constexpr int TARGET_FPS = 60; // render/input-poll rate; game logic still steps once per GAME_TICK_SECONDS
```

```cpp
input.pollFrame();
accumulatedTime += GetFrameTime();
while (accumulatedTime >= GAME_TICK_SECONDS) {
    accumulatedTime -= GAME_TICK_SECONDS;
    if (runOneTick()) return;
}
renderFrame();
```

The `logic/` layer has no dependency on raylib at all: `HumanPlayer` reads input through the `IInputProvider` interface, and only the `handlers/` layer's `RaylibInputProvider` knows about raylib's actual key-polling API. The same separation lets `ComputerPlayer` and `SearchAgentPlayer` plug into `GameController` as interchangeable `Player` implementations, without either the game loop or the rendering code needing to change.

---

## Installation

Requirements: CMake 3.27+, and a C++20-capable compiler. `raylib` and `raylib-cpp` are fetched automatically via CMake's `FetchContent` if not already installed.

Clone the repository:

```bash
git clone https://github.com/TalGershanov/tank_battle.git
cd tank_battle
```

Configure and build:

```bash
cmake -B build
cmake --build build
```

---

## Usage

Run the built executable (`tankBattle`, located in the build directory):

```bash
./build/tankBattle
```

### Main menu

```text
(1) Start a new game
(8) Show controls
(9) EXIT
```

### Opponent selection

```text
(1) Human vs Human
(2) Human vs Computer
```

### Difficulty selection

Shown only after choosing Human vs Computer:

```text
(1) Easy
(2) Hard
```

### Controls

| Action              | Player 1 | Player 2 |
|---------------------|----------|----------|
| Left track forward   | Q        | U        |
| Left track backward  | A        | J        |
| Right track forward  | E        | O        |
| Right track backward | D        | L        |
| Stay                 | S        | K        |
| Fire                 | W        | I        |
| Switch active tank   | Z        | M        |

While in a match, press `ESC` to pause (press again to resume, or `X` to quit to the main menu).

---

## Roadmap

### Phase 1 — Core Prototype

* [x] Board, tank, shell, wall, and mine data model
* [x] Tank movement via independent track control
* [x] Shooting, cooldowns, and cannon-loss mechanic
* [x] Destructible walls
* [x] Mines with a visibility mechanic
* [x] Full collision resolution
* [x] Human vs Human and Human vs Computer modes
* [x] Main menu, opponent selection, and controls screen
* [x] Win/tie detection and pause menu

### Phase 2 — Raylib Port & Layered Refactor

* [x] Split into `data / logic / handlers` layers
* [x] Port rendering to raylib
* [x] Platform-agnostic input abstraction (`IInputProvider`)
* [x] Fixed-timestep game loop decoupled from rendering

### Phase 3 — Difficulty & AI

* [x] Difficulty selection screen (Easy / Hard)
* [x] Alpha-beta pruned minimax AI for Hard difficulty

### Phase 4 — Polish

* [ ] Improved visuals
* [ ] Configurable number of tanks per player

---

## Status

Tank Battle is an actively developed project. The core gameplay loop — movement, shooting, walls, mines, collisions, menus, and both a rule-based and a search-based AI opponent — is complete and playable. The next milestone is Phase 4 polish: improved visuals and a configurable tank count.

---

## Author

**Tal Gershanov**

GitHub: https://github.com/TalGershanov/tank_battle
