#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

#include "Play.h"
#include "MainGame.h"

std::vector <Tile> vTiles;
std::vector <Ghost> vGhosts;

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
	static float totalGameTime = 0.f;
	totalGameTime += elapsedTime;

	UpdateGameStates();

	Draw();

	ActivateGhost(totalGameTime);

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
			UpdateGhosts();
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

	id = Play::CreateGameObject(TYPE_GHOST, { vTiles[BLINKY_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[BLINKY_SPAWN_POS].pos.y }, 15, SPR_BLINKY);
	Ghost blinky;
	blinky.id = id;
	blinky.type = GHOST_BLINKY;
	blinky.state = GHOST_IDLE;
	blinky.dir = DIR_UP;

	vGhosts.push_back(blinky);
}

void ActivateGhost(float time)
{
	for (Ghost& ghost : vGhosts)
	{
		switch (ghost.type)
		{
			case GHOST_BLINKY:
			{
				if (time > 1.0f)
					ghost.activated = true;

				break;
			}
			case GHOST_PINKY:
			{
				if (time > 5.0f)
					ghost.activated = true;
				break;
			}
			case GHOST_INKY:
			{
				if (time > 10.0f)
					ghost.activated = true;
				break;
			}
			case GHOST_CLYDE:
			{
				if (time > 15.0f)
					ghost.activated = true;
				break;
			}
		}
		
	}
}

void Draw()
{
	Play::ClearDrawingBuffer(Play::cBlack);

	Play::DrawSprite(SPR_BACKGROUND, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 1);

	GameObject& pacman = Play::GetGameObjectByType(TYPE_PACMAN);

	Play::DrawObjectRotated(pacman);
	//Play::DrawCircle(pacman.pos, 15, Play::cWhite);

	std::vector <int> vGhosts = Play::CollectGameObjectIDsByType(TYPE_GHOST);

	for (int ghostId : vGhosts)
	{
		GameObject& ghost = Play::GetGameObject(ghostId);
		Play::DrawObjectRotated(ghost);
		//Play::DrawCircle(ghost.pos, 15, Play::cRed);
	}

	// Draw Tile Map //
	/*for (Tile& tile : vTiles)
		Play::DrawRect({ tile.pos.x - TILE_SIZE / 2, tile.pos.y - TILE_SIZE / 2 }, { tile.pos.x + TILE_SIZE / 2, tile.pos.y + TILE_SIZE / 2 }, Play::cWhite);*/


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

	Play::DrawFontText( "64", "next: " + std::to_string(vGhosts[0].nextTile), { DISPLAY_WIDTH - 150, 50}, Play::CENTRE);
	Play::DrawFontText( "64", "target: " + std::to_string(vGhosts[0].targetTile), { DISPLAY_WIDTH - 150, 100 }, Play::CENTRE );
	Play::DrawFontText( "64", "current: " + std::to_string(vGhosts[0].currentTile), { DISPLAY_WIDTH - 150, 150 }, Play::CENTRE );
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

void UpdateGhosts()
{
	for (Ghost& ghost : vGhosts)
	{
		GameObject& objGhost = Play::GetGameObject(ghost.id);

		GhostAI(ghost);

		Play::UpdateGameObject(objGhost);
	}
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

void GhostAI(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);
	switch (ghost.state)
	{
		case GHOST_IDLE:
		{
			if (ghost.activated)
				ExitGhostHouse(ghost);

			break;
		}
		case GHOST_CHASE:
		{
			GhostNextTileReached(ghost);
			//SetChaseTarget(ghost);

			if (ghost.nextTile == ghost.currentTile)
				SetGhostDirection(ghost);

			SetGhostNextTile(ghost);
			GhostMovement(ghost);
			break;
		}
		case GHOST_SCATTER:
		{
			if (IsGhostTargetReached(ghost))
				SetScatterTarget(ghost);

			break;
		}
		case GHOST_FRIGHTENED:
		{
			if (IsGhostTargetReached(ghost))
				ghost.targetTile = Play::RandomRoll(vTiles.size());

			break;
		}
	}
}

void SetChaseTarget(Ghost& ghost)
{
	switch (ghost.type)
	{
		case GHOST_BLINKY:
		{
			ghost.targetTile = pacman.currentTile;
			break;
		}
		case GHOST_PINKY:
		{
			ghost.targetTile = pacman.currentTile;
			break;
		}
		case GHOST_INKY:
		{
			ghost.targetTile = pacman.currentTile - 3;
			break;
		}
		case GHOST_CLYDE:
		{
			ghost.targetTile = pacman.currentTile + 3;
			break;
		}
	}
}

void SetScatterTarget(Ghost& ghost)
{
	switch (ghost.type)
	{
		case GHOST_BLINKY:
		{
			break;
		}
		case GHOST_PINKY:
		{
			break;
		}
		case GHOST_INKY:
		{
			break;
		}
		case GHOST_CLYDE:
		{
			break;
		}
	}
}

void SetGhostDirection(Ghost& ghost)
{
	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (vTiles[ghost.currentTile - BOARD_SIZE.x].type == TILE_WALL)
			{
				if (vTiles[ghost.currentTile - 1].type == TILE_WALL)
					ghost.dir = DIR_RIGHT;
				else
					ghost.dir = DIR_LEFT;
			}
			else
				ghost.dir = DIR_UP;

			break;
		}
		case DIR_DOWN:
		{
			if (vTiles[ghost.currentTile + BOARD_SIZE.x].type == TILE_WALL)
			{
				if (vTiles[ghost.currentTile - 1].type == TILE_WALL)
					ghost.dir = DIR_RIGHT;
				else
					ghost.dir = DIR_LEFT;
			}
			else
				ghost.dir = DIR_DOWN;

			break;
		}
		case DIR_LEFT:
		{
			if (vTiles[ghost.currentTile - 1].type == TILE_WALL)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type == TILE_WALL)
					ghost.dir = DIR_DOWN;
				else
					ghost.dir = DIR_UP;
			}
			else
				ghost.dir = DIR_LEFT;

			break;
		}
		case DIR_RIGHT:
		{
			if (vTiles[ghost.currentTile + 1].type == TILE_WALL)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type == TILE_WALL)
					ghost.dir = DIR_DOWN;
				else
					ghost.dir = DIR_UP;
			}
			else
				ghost.dir = DIR_RIGHT;

			break;
		}
	}
}

void SetGhostNextTile(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (!vTiles[ghost.currentTile - BOARD_SIZE.x].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile - BOARD_SIZE.x;
				vTiles[ghost.currentTile].ghostOccupied = false;
				//objGhost.velocity = GHOST_VELOCITY_Y * (-1);
			}
			break;
		}
		case DIR_DOWN:
		{
			if (!vTiles[ghost.currentTile + BOARD_SIZE.x].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile + BOARD_SIZE.x;
				vTiles[ghost.currentTile].ghostOccupied = false;
				//objGhost.velocity = GHOST_VELOCITY_Y;
			}
			break;
		}
		case DIR_LEFT:
		{
			if (!vTiles[ghost.currentTile - 1].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile - 1;
				vTiles[ghost.currentTile].ghostOccupied = false;
				//objGhost.velocity = GHOST_VELOCITY_X * (-1);
			}
			break;
		}
		case DIR_RIGHT:
		{
			if (!vTiles[ghost.currentTile + 1].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile + 1;
				vTiles[ghost.currentTile].ghostOccupied = false;
				//objGhost.velocity = GHOST_VELOCITY_X;
			}
			
			break;
		}
	}
}

void GhostMovement(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			objGhost.velocity = GHOST_VELOCITY_Y * (-1);
			break;
		}
		case DIR_DOWN:
		{
			objGhost.velocity = GHOST_VELOCITY_Y;
			break;
		}
		case DIR_LEFT:
		{
			objGhost.velocity = GHOST_VELOCITY_X * (-1);
			break;
		}
		case DIR_RIGHT:
		{
			objGhost.velocity = GHOST_VELOCITY_X;
			break;
		}
	}
}

void GhostNextTileReached(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (objGhost.pos.y < vTiles[ghost.nextTile].pos.y)
				GhostSettled(ghost);

			break;
		}
		case DIR_DOWN:
		{
			if (objGhost.pos.y > vTiles[ghost.nextTile].pos.y)
				GhostSettled(ghost);

			break;
		}
		case DIR_LEFT:
		{
			if (objGhost.pos.x < vTiles[ghost.nextTile].pos.x)
				GhostSettled(ghost);
		
			break;
		}
		case DIR_RIGHT:
		{
			if (objGhost.pos.x > vTiles[ghost.nextTile].pos.x)
				GhostSettled(ghost);

			break;
		}
	}
}

void GhostSettled(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	objGhost.pos = vTiles[ghost.nextTile].pos;
	vTiles[ghost.nextTile].ghostOccupied = true;
	ghost.currentTile = ghost.nextTile;
}

bool IsGhostTargetReached(Ghost& ghost)
{
	if (ghost.currentTile == ghost.targetTile)
		return true;

	return false;
}

void ExitGhostHouse(Ghost& ghost)
{
	switch (ghost.type)
	{
		case GHOST_BLINKY:
		{
			GameObject& objGhost = Play::GetGameObject(ghost.id);

			if (objGhost.pos.y < vTiles[GHOST_EXIT_POS].pos.y)
			{
				objGhost.pos = vTiles[GHOST_EXIT_POS].pos;
				vTiles[GHOST_EXIT_POS].ghostOccupied = true;
				ghost.currentTile = GHOST_EXIT_POS;
				ghost.targetTile = GHOST_EXIT_POS;
				ghost.nextTile = GHOST_EXIT_POS;
				ghost.state = GHOST_CHASE;
			}

			objGhost.velocity = GHOST_VELOCITY_Y * (-1);
			break;
		}
		case GHOST_PINKY:
		{
			break;
		}
		case GHOST_INKY:
		{
			break;
		}
		case GHOST_CLYDE:
		{
			break;
		}
	}
}



