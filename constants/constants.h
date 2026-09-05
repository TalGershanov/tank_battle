#pragma once

// =====================================================================
// Direction
// =====================================================================
enum class Direction {UP, UP_LEFT, LEFT, DOWN_LEFT, DOWN, DOWN_RIGHT, RIGHT, UP_RIGHT, NONE};
constexpr int NUM_DIRECTIONS = 8; // number of real (non-NONE) directions

// =====================================================================
// Board / geometry
// =====================================================================
constexpr int BOARD_WIDTH = 40;   // columns
constexpr int BOARD_HEIGHT = 20;  // rows
constexpr int MIN_ROW = 1;
constexpr int MAX_ROW = BOARD_HEIGHT - 2;
constexpr int MIN_COL = 1;
constexpr int MAX_COL = BOARD_WIDTH - 2;
constexpr char BOARDER_SYMBOL = '+';

// =====================================================================
// Symbols
// =====================================================================
constexpr char TANK_SYMBOL_PLAYER_1 = '1';
constexpr char TANK_SYMBOL_PLAYER_2 = '2';
constexpr char SHELL_SYMBOL = '*';
constexpr char STRONG_WALL_SYMBOL = '#';
constexpr char WEAK_WALL_SYMBOL = 'H';
constexpr char MINE_SYMBOL = '@';
constexpr char CANNON_UP_OR_DOWN_SYMBOL = '|';
constexpr char CANNON_UL_OR_DR_SYMBOL = '\\';
constexpr char CANNON_DL_OR_UR_SYMBOL = '/';
constexpr char CANNON_L_OR_R_SYMBOL = '-';
constexpr char EMPTY_CELL_SYMBOL = ' ';

// =====================================================================
// Gameplay tuning
// =====================================================================
constexpr int MAX_COOLDOWN = 5;
constexpr int MIN_COOLDOWN = 0;

constexpr int MAX_WALL_HITS_NUM = 2;
constexpr int WALL_HITS_STRONG = 0; // hitsNumber at which a wall shows STRONG_WALL_SYMBOL
constexpr int WALL_HITS_WEAK = 1;   // hitsNumber at which a wall shows WEAK_WALL_SYMBOL

constexpr int NUM_OF_MINES = 6;

constexpr int TANK1_INIT_POS_X = BOARD_HEIGHT / 4;
constexpr int TANK1_INIT_POS_Y = BOARD_WIDTH / 4;
constexpr int TANK2_INIT_POS_X = (BOARD_HEIGHT * 3) / 4;
constexpr int TANK2_INIT_POS_Y = (BOARD_WIDTH * 3) / 4;

constexpr int PLAYER2_SPAWN_OFFSET_ROW = 3;
constexpr int PLAYER2_SPAWN_OFFSET_COL = 3;

// =====================================================================
// AI tuning
// =====================================================================
constexpr int NUM_AI_ACTIONS = 5;   // rotate-left, rotate-right, forward, backward, stay
constexpr int NUM_EVADE_OPTIONS = 2; // rotate-left vs rotate-right
constexpr int SHELL_CHASE_LOOKAHEAD_STEPS = 10; // how far ahead to project a shell when checking if it threatens a tank

// =====================================================================
// Search Agent AI tuning (SearchAgentPlayer, minimax w/ alpha-beta)
// =====================================================================
constexpr int SEARCH_AGENT_SEARCH_ROUNDS = 2;          // minimax depth, in full MAX+MIN rounds
constexpr int SEARCH_AGENT_REPLAN_TICKS = 2;            // full re-search cadence, in game ticks (~1s)
constexpr int SEARCH_AGENT_SHELL_LOOKAHEAD_STEPS = 10;  // shell-threat projection horizon
constexpr double SEARCH_AGENT_SWITCH_MARGIN = 150.0;    // min quickSelfEvaluate() gap before switching active tank

constexpr double SEARCH_AGENT_WIN_SCORE = 100000.0;
constexpr double SEARCH_AGENT_LINE_OF_FIRE_OFFENSE_WEIGHT = 400.0;
constexpr double SEARCH_AGENT_LINE_OF_FIRE_DEFENSE_WEIGHT = 600.0;
constexpr double SEARCH_AGENT_SHELL_THREAT_OFFENSE_WEIGHT = 250.0;
constexpr double SEARCH_AGENT_SHELL_THREAT_DEFENSE_WEIGHT = 350.0;
constexpr double SEARCH_AGENT_MINE_PROXIMITY_WEIGHT = 400.0;
constexpr double SEARCH_AGENT_DISTANCE_WEIGHT = 2.0;
constexpr double SEARCH_AGENT_WALL_COVER_WEIGHT = 20.0;

// =====================================================================
// Key bindings
//
// Values are chosen to match raylib's KEY_* codes directly (raylib
// deliberately aligns its letter/digit key codes with plain ASCII), so
// the logic layer can stay free of any raylib #include while still
// using the real values the concrete IInputProvider will be asked about.
// KEY_ESCAPE in raylib is 256 (not ASCII 27), so it's named separately.
// =====================================================================
struct KeyBindings {
    int leftForwardKey;
    int leftBackwardKey;
    int rightForwardKey;
    int rightBackwardKey;
    int stayKey;
    int fireKey;
    int switchTankKey;
};

constexpr KeyBindings PLAYER1_KEYS{'Q', 'A', 'E', 'D', 'S', 'W', 'Z'};
constexpr KeyBindings PLAYER2_KEYS{'U', 'J', 'O', 'L', 'K', 'I', 'M'};

constexpr int ESCAPE_KEY_CODE = 256; // raylib KEY_ESCAPE
constexpr int QUIT_KEY_CODE = 'X';   // raylib KEY_X

constexpr int MENU_START_GAME_KEY = '1';
constexpr int MENU_SHOW_INSTRUCTIONS_KEY = '8';
constexpr int MENU_EXIT_KEY = '9';
constexpr int OPPONENT_HUMAN_KEY = '1';
constexpr int OPPONENT_COMPUTER_KEY = '2';
constexpr int DIFFICULTY_EASY_KEY = '1';
constexpr int DIFFICULTY_HARD_KEY = '2';

// =====================================================================
// Mine positions
// =====================================================================
constexpr int MINE_POS_X[NUM_OF_MINES] = {
        BOARD_HEIGHT / 6,
        (BOARD_HEIGHT * 2) / 6,
        (BOARD_HEIGHT * 3) / 6,
        (BOARD_HEIGHT * 4) / 6,
        (BOARD_HEIGHT * 5) / 6,
        MAX_ROW
};

constexpr int MINE_POS_Y[NUM_OF_MINES] = {
        BOARD_WIDTH / 6,
        (BOARD_WIDTH * 2) / 6,
        (BOARD_WIDTH * 3) / 6,
        (BOARD_WIDTH * 4) / 6,
        (BOARD_WIDTH * 5) / 6,
        MAX_COL
};

// =====================================================================
// Wall layout table
// =====================================================================
struct WallCoord { int row; int col; };

constexpr WallCoord WALL_LAYOUT[] = {
        {10, 10}, {10, 11}, {10, 12}, {10, 13},

        {14, 16}, {14, 17}, {14, 18}, {14, 19}, {14, 20}, {14, 21},

        {5, 7}, {6, 7}, {7, 7},

        {2, MAX_COL - 7}, {2, MAX_COL - 8}, {2, MAX_COL - 9},
        {2, MAX_COL - 10}, {2, MAX_COL - 11}, {2, MAX_COL - 12},

        {10, 32}, {10, 33}, {10, 34}, {10, 35},

        {12, 37}, {13, 37}, {14, 37}, {15, 37},

        {MIN_ROW + 1, MAX_COL - 3}, {MIN_ROW + 2, MAX_COL - 3}, {MIN_ROW + 3, MAX_COL - 3},
        {MIN_ROW + 4, MAX_COL - 3}, {MIN_ROW + 5, MAX_COL - 3}, {MIN_ROW + 6, MAX_COL - 3},

        {MAX_ROW - 1, MIN_COL + 3}, {MAX_ROW - 2, MIN_COL + 3}, {MAX_ROW - 3, MIN_COL + 3},
        {MAX_ROW - 4, MIN_COL + 3}, {MAX_ROW - 5, MIN_COL + 3}, {MAX_ROW - 6, MIN_COL + 3},
};
constexpr int NUM_WALLS = sizeof(WALL_LAYOUT) / sizeof(WallCoord);

// =====================================================================
// Rendering
// =====================================================================
constexpr int CELL_PIXEL_SIZE = 20;
constexpr int HUD_PANEL_WIDTH_PX = 320;
constexpr int WINDOW_WIDTH_PX = BOARD_WIDTH * CELL_PIXEL_SIZE + HUD_PANEL_WIDTH_PX;
constexpr int WINDOW_HEIGHT_PX = BOARD_HEIGHT * CELL_PIXEL_SIZE;
constexpr int HUD_PANEL_X_PX = BOARD_WIDTH * CELL_PIXEL_SIZE + 20;
constexpr int HUD_MARGIN_PX = 20;
constexpr int HUD_LINE_HEIGHT_PX = 22;
constexpr int FONT_POINT_SIZE = 20;
constexpr const char* WINDOW_TITLE = "Tank Battle";

constexpr double GAME_TICK_SECONDS = 0.5;
constexpr int TARGET_FPS = 60; // render/input-poll rate; game logic still steps once per GAME_TICK_SECONDS
