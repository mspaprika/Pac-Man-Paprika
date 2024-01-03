#pragma once

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

// Sprites' names
const char* SPR_BACKGROUND = "back";

const char* SPR_PACMAN = "spr_pacman";
const char* SPR_BLINKY = "spr_ghost_blinky";
const char* SPR_PINKY = "spr_ghost_pinky";
const char* SPR_INKY = "spr_ghost_inky";
const char* SPR_CLYDE = "spr_ghost_clyde";
const char* SPR_VULNERABLE = "spr_ghost_vulnerable";

const char* SPR_DOT = "dot";
const char* SPR_POWER = "spr_power";

const char* SPR_CARROT = "carrot";
const char* SPR_CHERRY = "cherry";
const char* SPR_APPLE = "apple";

// Pacman
const int PACMAN_SPAWN_POS = { 658 };
const int PAC_RESPAWN_POS_LEFT = { 392 };
const int PAC_RESPAWN_POS_RIGHT = { 419 };

const float PACMAN_SPEED = { 1.5f };
const Vector2f PACMAN_VELOCITY_X = { PACMAN_SPEED, 0.0f };
const Vector2f PACMAN_VELOCITY_Y = { 0.0f, PACMAN_SPEED };

// Board
const int TILE_SIZE = 20;
const int BOARD_OFFSET_X = 370;
const int BOARD_OFFSET_Y = 70;

const Vector2D BOARD_SIZE = { 28, 31 };

const int BOARD_LIM_LEFT = BOARD_OFFSET_X;
const int BOARD_LIM_RIGHT = BOARD_OFFSET_X + ( BOARD_SIZE.x * TILE_SIZE );



int BOARD[31][28] =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 3, 3, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 3, 3, 3, 3, 3, 3, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 1, 3, 3, 3, 3, 3, 3, 1, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 3, 3, 3, 3, 3, 3, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
	{ 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

enum TileType
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_GHOST_SPAWN,
};

struct Tile
{
	int id;
	Point2D pos;
	TileType type;
	bool occupied = false;
};

enum GameFlow
{
	STATE_IDLE,
	STATE_PLAY,
	STATE_GAME_OVER,
	STATE_WIN,
	STATE_PAUSE,
};

enum PacmanState
{
	PAC_IDLE,
	PAC_MOVE_LEFT,
	PAC_MOVE_RIGHT,
	PAC_MOVE_UP,
	PAC_MOVE_DOWN,
	PAC_DYING,
};

enum GameObjectType
{
	TYPE_PACMAN,
	TYPE_GHOST,
	TYPE_DOT,
	TYPE_POWER,
};

enum GhostType
{
	GHOST_BLINKY,
	GHOST_PINKY,
	GHOST_INKY,
	GHOST_CLYDE,
};

enum Direction
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
};

struct GameState
{
	int score{ 0 };
	float powerTimer{ 0.0f };
	bool vulnerable{ false };
	GameFlow state = STATE_IDLE;
	PacmanState pState = PAC_IDLE;
};

struct Pacman
{
	Point2D pos;

	Direction dir;
	Direction nextDir;

	int currentRow;
	int currentTile = PACMAN_SPAWN_POS;
	int nextTile = PACMAN_SPAWN_POS	;
};

// Functions' declarations
void UpdateGameStates();
void Draw();
void CreateTiles();
void CreateGameObjects();

void UpdatePacman();
void UpdateDots();
void PacmanMainControlls();

void SweepNextTile(int id, int oldID);
void DrawGameStats();

void PacmanTargetReached(GameObject& objPacman);