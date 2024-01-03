#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

#include "Play.h"
#include "MainGame.h"

std::vector <Tile> vTiles;

GameState gState;
Pacman pacman;

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();

	CreateTiles();
	CreateGameObjects();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	UpdateGameStates();

	Draw();

	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}


void UpdateGameStates()
{
	switch (gState.state)
	{
		case STATE_IDLE:
		{

		}
		case STATE_PLAY:
		{
			UpdatePacman();
			UpdateDots();
		}
		case STATE_GAME_OVER:
		{

		}
		case STATE_PAUSE:
		{

		}
	}
}

void CreateGameObjects()
{
	Point2D pos = vTiles[PACMAN_SPAWN_POS].pos;
	vTiles[PACMAN_SPAWN_POS].occupied = true;

	int id = Play::CreateGameObject(TYPE_PACMAN, pos, 15, SPR_PACMAN);

	Pacman pacman;
}

void Draw()
{
	Play::ClearDrawingBuffer(Play::cBlack);

	Play::DrawSprite(SPR_BACKGROUND, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 1);

	GameObject& pacman = Play::GetGameObjectByType(TYPE_PACMAN);

	Play::DrawObjectRotated(pacman);
	//Play::DrawCircle(pacman.pos, 15, Play::cWhite);


	for (int i = 0; i < BOARD_SIZE.x; i++)
	{
		for (int j = 0; j < BOARD_SIZE.y; j++)
		{
			Point2D tilePos = { BOARD_OFFSET_X + i * TILE_SIZE, BOARD_OFFSET_Y + j * TILE_SIZE };
			//Play::DrawRect({ tilePos.x - TILE_SIZE / 2, tilePos.y - TILE_SIZE / 2 }, { tilePos.x + TILE_SIZE / 2, tilePos.y + TILE_SIZE / 2 }, Play::cWhite);
		}
	}

	std::vector <int> vDots =  Play::CollectGameObjectIDsByType(TYPE_DOT);

	for ( int dotId : vDots)
	{
		GameObject& dot = Play::GetGameObject(dotId);
		Play::DrawObject(dot);
		//Play::DrawCircle(dot.pos, 5, Play::cWhite);
	}

	DrawGameStats();

	Play::PresentDrawingBuffer();
}

void DrawGameStats()
{
	Play::DrawFontText( "64", "SCORE: " + std::to_string(gState.score), { DISPLAY_WIDTH / 2, 30 }, Play::CENTRE );
}

void CreateTiles()
{
	int tileId = 0;

	for (int j = 0; j < BOARD_SIZE.y; j++)
	{
		for (int i = 0; i < BOARD_SIZE.x; i++)
		{
			Point2D tilePos = { BOARD_OFFSET_X + i * TILE_SIZE, BOARD_OFFSET_Y + j * TILE_SIZE };
			
			Tile tile;
			tile.pos = tilePos;
			tile.id = tileId;

			switch (BOARD[j][i])
			{
				case 0:
				{
					tile.type = TILE_EMPTY;
					Play::CreateGameObject(TYPE_DOT, tile.pos, 5, SPR_DOT);
					break;
				}
				case 1:
				{
					tile.type = TILE_WALL;
					break;
				}
				case 2:
				{
					tile.type = TILE_EMPTY;
					break;
				}
				case 3:
				{
					tile.type = TILE_GHOST_SPAWN;
					break;
				}
			}

			vTiles.push_back(tile);
			tileId++;
		}
	}
}

void UpdatePacman()
{
	GameObject& objPacman = Play::GetGameObjectByType(TYPE_PACMAN);

	switch (gState.pState)
	{
		case PAC_IDLE:
		{
			objPacman.velocity = { 0, 0 };
			objPacman.animSpeed = 0.f;
			objPacman.frame = 0;
			PacmanMainControlls();

			break;
		}
		case PAC_MOVE_LEFT:
		{
			if (objPacman.pos.x < vTiles[pacman.nextTile].pos.x)
				PacmanTargetReached(objPacman);

			if (pacman.currentTile == pacman.nextTile)
			{
				if (pacman.currentTile == PAC_RESPAWN_POS_LEFT)
					SweepNextTile(PAC_RESPAWN_POS_RIGHT, pacman.currentTile);
				else
					SweepNextTile(pacman.currentTile - 1, pacman.currentTile);
			}
			
			objPacman.velocity = PACMAN_VELOCITY_X * (-1);
			objPacman.rotation = 3.14f;
			objPacman.animSpeed = 0.1f;

			PacmanMainControlls();

			if (objPacman.pos.x < BOARD_LIM_LEFT)
				objPacman.pos.x = BOARD_LIM_RIGHT;

			break;
		}
		case PAC_MOVE_RIGHT:
		{
			if (objPacman.pos.x > vTiles[pacman.nextTile].pos.x)
				PacmanTargetReached(objPacman);

			if (pacman.currentTile == pacman.nextTile)
			{
				if (pacman.currentTile == PAC_RESPAWN_POS_RIGHT)
					SweepNextTile(PAC_RESPAWN_POS_LEFT, pacman.currentTile);
				else
					SweepNextTile(pacman.currentTile + 1, pacman.currentTile);
			}

			objPacman.velocity = PACMAN_VELOCITY_X;
			objPacman.rotation = 0.f;
			objPacman.animSpeed = 0.1f;

			PacmanMainControlls();

			if (objPacman.pos.x > BOARD_LIM_RIGHT)
				objPacman.pos.x = BOARD_LIM_LEFT;

			break;
		}
		case PAC_MOVE_UP:
		{
			if (objPacman.pos.y < vTiles[pacman.nextTile].pos.y)
				PacmanTargetReached(objPacman);

			if (pacman.currentTile == pacman.nextTile)
				SweepNextTile(pacman.currentTile - BOARD_SIZE.x, pacman.currentTile);


			objPacman.velocity = PACMAN_VELOCITY_Y * (-1);
			objPacman.rotation = 4.71f;
			objPacman.animSpeed = 0.1f;

			PacmanMainControlls();
			break;
		}
		case PAC_MOVE_DOWN:
		{
			if (objPacman.pos.y > vTiles[pacman.nextTile].pos.y)
				PacmanTargetReached(objPacman);

			if (pacman.currentTile == pacman.nextTile)
				SweepNextTile(pacman.currentTile + BOARD_SIZE.x, pacman.currentTile);

			objPacman.velocity = PACMAN_VELOCITY_Y;
			objPacman.rotation = 1.57f;
			objPacman.animSpeed = 0.1f;

			PacmanMainControlls();
			break;
		}
	}

	Play::UpdateGameObject(objPacman);
}

void PacmanTargetReached(GameObject& objPacman)
{
	objPacman.pos = vTiles[pacman.nextTile].pos;
	vTiles[pacman.nextTile].occupied = true;
	pacman.currentTile = pacman.nextTile;
}

void PacmanMainControlls()
{
	if (Play::KeyDown(VK_LEFT))
	{
		gState.pState = PAC_MOVE_LEFT;
	}
	else if (Play::KeyDown(VK_RIGHT))
	{
		gState.pState = PAC_MOVE_RIGHT;
	}
	else if (Play::KeyDown(VK_UP))
	{
		gState.pState = PAC_MOVE_UP;
	}
	else if (Play::KeyDown(VK_DOWN))
	{
		gState.pState = PAC_MOVE_DOWN;
	}
}

void UpdateDots()
{
	std::vector <int> vDots = Play::CollectGameObjectIDsByType(TYPE_DOT);

	for (int dotId : vDots)
	{
		GameObject& dot = Play::GetGameObject(dotId);

		if (Play::IsColliding(dot, Play::GetGameObjectByType(TYPE_PACMAN)))
		{
			Play::DestroyGameObject(dotId);
			gState.score += 10;
		}

		Play::UpdateGameObject(dot);
	}
}

void SweepNextTile(int id, int oldID)
{
	if (vTiles[id].type == TILE_WALL || vTiles[id].type == TILE_GHOST_SPAWN)
		gState.pState = PAC_IDLE;

	else if (!vTiles[id].occupied)
	{
		pacman.nextTile = id;
		vTiles[oldID].occupied = false;
	}
}






