#pragma once

#include "Play.h"

constexpr int DISPLAY_WIDTH = 1280;
constexpr int DISPLAY_HEIGHT = 720;
constexpr int DISPLAY_SCALE = 1;

// Sprites' names
const char* SPR_BACKGROUND = "back";

const char* SPR_PACMAN = "spr_pacman_right";
const char* SPR_PACMAN_DEAD = "spr_pacman_dead";
const char* SPR_VULNERABLE = "ghost_vulnerable";

const char* SPR_BLINKY_MOVE_UP = "ghost_blinky_up";
const char* SPR_BLINKY_MOVE_DOWN = "ghost_blinky_down";
const char* SPR_BLINKY_MOVE_LEFT = "ghost_blinky_left";
const char* SPR_BLINKY_MOVE_RIGHT = "ghost_blinky_right";

const char* SPR_CLYDE_MOVE_UP = "ghost_clyde_up";
const char* SPR_CLYDE_MOVE_DOWN = "ghost_clyde_down";
const char* SPR_CLYDE_MOVE_LEFT = "ghost_clyde_left";
const char* SPR_CLYDE_MOVE_RIGHT = "ghost_clyde_right";

const char* SPR_PINKY_MOVE_UP = "ghost_pinky_up";
const char* SPR_PINKY_MOVE_DOWN = "ghost_pinky_down";
const char* SPR_PINKY_MOVE_LEFT = "ghost_pinky_left";
const char* SPR_PINKY_MOVE_RIGHT = "ghost_pinky_right";

const char* SPR_INKY_MOVE_UP = "ghost_inky_up";
const char* SPR_INKY_MOVE_DOWN = "ghost_inky_down";
const char* SPR_INKY_MOVE_LEFT = "ghost_inky_left";
const char* SPR_INKY_MOVE_RIGHT = "ghost_inky_right";


const char* SPR_DOT = "dot";
const char* SPR_POWER = "power";

const char* SPR_CARROT = "carrot";
const char* SPR_CHERRY = "cherry";
const char* SPR_APPLE = "apple";
const char* SPR_PEPPER = "pepper";
const char* SPR_PAPRIKA = "paprika";


// Pacman
const int PACMAN_SPAWN_POS = { 658 };
const int PAC_RESPAWN_POS_LEFT = { 392 };
const int PAC_RESPAWN_POS_RIGHT = { 419 };

const float PACMAN_SPEED = { 2.f };
const Vector2f PACMAN_VELOCITY_X = { PACMAN_SPEED, 0.0f };
const Vector2f PACMAN_VELOCITY_Y = { 0.0f, PACMAN_SPEED };
const float PACMAN_ANIM_SPEED = { 0.3f };

// Ghosts
const int GHOST_RESPAWN_POS = { 391 };
const int INKY_SPAWN_POS = { 404 };
const int CLYDE_SPAWN_POS = { 407 };
const int PINKY_SPAWN_POS = { 405 };
const int BLINKY_SPAWN_POS = { 321 };

const float GHOST_SPEED = { 1.5f };
const float GHOST_DEAD_SPEED = { 3.0f };

const Vector2f GHOST_VELOCITY_X = { GHOST_SPEED, 0.0f };
const Vector2f GHOST_VELOCITY_Y = { 0.0f, GHOST_SPEED };

const int GHOST_EXIT_POS = { 321 };
const int BLINKY_SCATTER_POS = { 0 };
const int PINKY_SCATTER_POS = { 27 };
const int INKY_SCATTER_POS = { 841 };
const int CLYDE_SCATTER_POS = { 867 };

// Board
const int TILE_SIZE = 20;
const int HALF_TILE = TILE_SIZE / 2;
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
	{ 1, 4, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 4, 1 },
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
	{ 1, 4, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 4, 1 },
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
	bool ghostOccupied = false;
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
	TYPE_DESTROYED,
	TYPE_FRUIT,
};

enum GhostType
{
	GHOST_BLINKY,
	GHOST_PINKY,
	GHOST_INKY,
	GHOST_CLYDE,
};

enum GhostState
{
	GHOST_IDLE,
	GHOST_CHASE,
	GHOST_SCATTER,
	GHOST_FRIGHTENED,
	GHOST_DEAD,
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
	float ghostTimer{ 0.0f };
	bool vulnerable{ false };
	bool scatter{ false };
	bool gameRestarted{ false };

	float time{ 0.0f };
	int lives{ 3 };
	int level{ 1 };
	int maxDots{ 0 };
	int ghostsEaten{ 0 };

	float pSpeed{ PACMAN_SPEED };
	float gSpeed{ GHOST_SPEED };

	GameFlow state = STATE_PLAY;
	PacmanState pState = PAC_IDLE;
};

struct Ghost
{
	GhostState state = GHOST_IDLE;
	GhostType type;

	Direction dir;
	Direction nextDir;

	int id;
	int currentTile;
	int nextTile;
	int targetTile;
	bool activated{ false };
	bool eaten{ false };
	bool returned{ false };
	bool exited{ false };

	const char* SPRITE;
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
void DrawGameObjects(int TYPE);
void CreateTiles();
void CreateGameObjects();
float GetDistance(Point2D pos1, Point2D pos2);
void ReverseDirection(Ghost& ghost);
void RestartGame();
void DestroyObjects(int TYPE);

void UpdatePacman();
void UpdateDots();
void UpdatePower();
void PacmanMainControlls();
void UpdateDestroyed();

void SweepNextTile(int id, int oldID);
void DrawGameStats();

void PacmanTargetReached(GameObject& objPacman);
void UpdateGhosts();
void GhostAI(Ghost& ghost);
void SetChaseTarget(Ghost& ghost);
void SetScatterTarget(Ghost& ghost);
bool IsGhostTargetReached(Ghost& ghost);
void SetGhostDirection(Ghost& ghost);
void SetGhostNextTile(Ghost& ghost);
void GhostMovement(Ghost& ghost);

void GhostNextTileReached(Ghost& ghost);
void ExitGhostHouse(Ghost& ghost);
void ReturnAndExitGhostHouse(Ghost& ghost);
void ActivateGhost(float time);
void GhostSettled(Ghost& ghost);
void SetGhostSprites(Ghost& ghost);
void GhostReadyToGo(Ghost& ghost, int pos);
void GhostScatterControl();
void RestartGhosts();

void VulnerableCollision(Ghost& ghost);
void ChaseCollision();