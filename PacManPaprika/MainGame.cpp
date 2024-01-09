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
	gState.time += elapsedTime;

	GhostScatterControl();
	UpdateGameStates();
	Draw();
	ActivateGhost(gState.time);

	if (Play::KeyPressed(VK_TAB))
		RestartGame();

	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}


// Main Game Functions //
void UpdateGameStates()
{
	switch (gState.state)
	{
		case STATE_IDLE:
		{
			break;
		}
		case STATE_PLAY:
		{
			UpdatePacman();
			UpdatePower();
			UpdateDots();
			UpdateGhosts();
			break;
		}
		case STATE_GAME_OVER:
		{
			break;
		}
		case STATE_PAUSE:
		{
			break;
		}
	}

	UpdateDestroyed();
}

void CreateGameObjects()
{
	Point2D pos = vTiles[PACMAN_SPAWN_POS].pos;
	vTiles[PACMAN_SPAWN_POS].occupied = true;

	int id = Play::CreateGameObject(TYPE_PACMAN, pos, 15, SPR_PACMAN);
	Pacman pacman;

	id = Play::CreateGameObject(TYPE_GHOST, { vTiles[BLINKY_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[BLINKY_SPAWN_POS].pos.y }, 15, SPR_BLINKY_MOVE_LEFT);
	Ghost blinky;
	blinky.id = id;
	blinky.type = GHOST_BLINKY;
	blinky.state = GHOST_IDLE;
	blinky.dir = DIR_LEFT;
	blinky.SPRITE = SPR_BLINKY_MOVE_LEFT;

	vGhosts.push_back(blinky);

	id = Play::CreateGameObject(TYPE_GHOST, { vTiles[PINKY_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[PINKY_SPAWN_POS].pos.y }, 15, SPR_PINKY_MOVE_UP);
	Ghost pinky;
	pinky.id = id;
	pinky.type = GHOST_PINKY;
	pinky.state = GHOST_IDLE;
	pinky.dir = DIR_UP;
	pinky.SPRITE = SPR_PINKY_MOVE_UP;

	vGhosts.push_back(pinky);

	id = Play::CreateGameObject(TYPE_GHOST, { vTiles[INKY_SPAWN_POS].pos.x - (TILE_SIZE / 2), vTiles[INKY_SPAWN_POS].pos.y }, 15, SPR_INKY_MOVE_RIGHT);
	Ghost inky;
	inky.id = id;
	inky.type = GHOST_INKY;
	inky.state = GHOST_IDLE;
	inky.dir = DIR_RIGHT;
	inky.SPRITE = SPR_INKY_MOVE_RIGHT;

	vGhosts.push_back(inky);

	id = Play::CreateGameObject(TYPE_GHOST, { vTiles[CLYDE_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[CLYDE_SPAWN_POS].pos.y }, 15, SPR_CLYDE_MOVE_LEFT);
	Ghost clyde;
	clyde.id = id;
	clyde.type = GHOST_CLYDE;
	clyde.state = GHOST_IDLE;
	clyde.dir = DIR_LEFT;
	clyde.SPRITE = SPR_CLYDE_MOVE_LEFT;

	vGhosts.push_back(clyde);
}

void Draw()
{
	Play::ClearDrawingBuffer(Play::cBlack);

	Play::DrawSprite(SPR_BACKGROUND, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 1);

	// Draw Tile Map //
	//for (Tile& tile : vTiles)
		//Play::DrawRect({ tile.pos.x - TILE_SIZE / 2, tile.pos.y - TILE_SIZE / 2 }, { tile.pos.x + TILE_SIZE / 2, tile.pos.y + TILE_SIZE / 2 }, Play::cWhite);

	DrawGameObjects(TYPE_PACMAN);
	DrawGameObjects(TYPE_POWER);
	DrawGameObjects(TYPE_DOT);
	DrawGameObjects(TYPE_GHOST);

	//Play::DrawCircle(pacman.pos, 15, Play::cWhite);
	//Play::DrawCircle(ghost.pos, 15, Play::cRed);
	//Play::DrawCircle(dot.pos, 5, Play::cWhite);

	DrawGameStats();

	Play::PresentDrawingBuffer();
}

void DrawGameObjects(int TYPE)
{
	std::vector <int> vObjects = Play::CollectGameObjectIDsByType(TYPE);

	for (int objId : vObjects)
	{
		GameObject& obj = Play::GetGameObject(objId);
		Play::DrawObjectRotated(obj);
	}
}

void DrawGameStats()
{
	Play::DrawFontText( "64", "SCORE: " + std::to_string(gState.score), { DISPLAY_WIDTH / 2, 30 }, Play::CENTRE );

	//Play::DrawFontText( "64", "next: " + std::to_string(vGhosts[0].nextTile), { DISPLAY_WIDTH - 150, 50}, Play::CENTRE);
	//Play::DrawFontText( "64", "pac state: " + std::to_string(gState.pState), { DISPLAY_WIDTH - 150, 100 }, Play::CENTRE );
	//Play::DrawFontText( "64", "direction: " + std::to_string(vGhosts[0].dir), { DISPLAY_WIDTH - 150, 150 }, Play::CENTRE );
	Play::DrawFontText( "64", "state: " + std::to_string(vGhosts[0].state), { DISPLAY_WIDTH - 150, 200 }, Play::CENTRE );
	//Play::DrawFontText("64", "time: " + std::to_string(gState.time), { DISPLAY_WIDTH - 150, 250 }, Play::CENTRE);
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
					gState.maxDots++;
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
				case 4:
				{
					tile.type = TILE_EMPTY;
					Play::CreateGameObject(TYPE_POWER, tile.pos, 5, SPR_POWER);
					break;
				}
			}

			vTiles.push_back(tile);
			tileId++;
		}
	}
}


// Update Game Objects //
void UpdatePacman()
{
	GameObject& objPacman = Play::GetGameObjectByType(TYPE_PACMAN);

	Vector2f vY = { 0.0f, gState.pSpeed };
	Vector2f vX = { gState.pSpeed, 0.0f };

	ChaseCollision();

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
			
			objPacman.velocity = vX * (-1);

			if (vTiles[pacman.currentTile - 1].type == TILE_EMPTY)
				objPacman.rotation = 3.14f;

			objPacman.animSpeed = PACMAN_ANIM_SPEED;

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

			objPacman.velocity = vX;

			if (vTiles[pacman.currentTile + 1].type == TILE_EMPTY)
				objPacman.rotation = 0.f;

			objPacman.animSpeed = PACMAN_ANIM_SPEED;

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


			objPacman.velocity = vY * (-1);

			if (vTiles[pacman.currentTile - BOARD_SIZE.x].type == TILE_EMPTY)
				objPacman.rotation = 4.71f;

			objPacman.animSpeed = PACMAN_ANIM_SPEED;

			PacmanMainControlls();
			break;
		}
		case PAC_MOVE_DOWN:
		{
			if (objPacman.pos.y > vTiles[pacman.nextTile].pos.y)
				PacmanTargetReached(objPacman);

			if (pacman.currentTile == pacman.nextTile)
				SweepNextTile(pacman.currentTile + BOARD_SIZE.x, pacman.currentTile);

			objPacman.velocity = vY;

			if (vTiles[pacman.currentTile + BOARD_SIZE.x].type == TILE_EMPTY)
				objPacman.rotation = 1.57f;

			objPacman.animSpeed = PACMAN_ANIM_SPEED;

			PacmanMainControlls();
			break;
		}
		case PAC_DYING:
		{
			Play::SetSprite(objPacman, SPR_PACMAN_DEAD, 0.1f);

			if (Play::IsAnimationComplete(objPacman))
				RestartGame();

			objPacman.velocity = { 0, 0 };

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

		if (ghost.currentTile >= GHOST_EXIT_POS)
			ghost.exited = true;
		
		GhostAI(ghost);

		Play::UpdateGameObject(objGhost);
	}
}

void UpdatePower()
{
	if (gState.vulnerable)
	{
		gState.powerTimer += 1.0f / 60.0f;

		if (gState.powerTimer > 10.0f)
			gState.vulnerable = false;		
	}

	std::vector <int> vPower = Play::CollectGameObjectIDsByType(TYPE_POWER);
	for (int powerId : vPower)
	{
		GameObject& power = Play::GetGameObject(powerId);

		if (Play::IsColliding(power, Play::GetGameObjectByType(TYPE_PACMAN)))
		{
			power.type = TYPE_DESTROYED;
			gState.score += 50;
			gState.vulnerable = true;
			gState.powerTimer = 0.0f;

			for (Ghost& ghost : vGhosts)
			{
				if (ghost.activated && ghost.exited)
				{
					ReverseDirection(ghost);
					ghost.targetTile = Play::RandomRoll(vTiles.size() - 100);
					ghost.state = GHOST_FRIGHTENED;
				}
			}
		}
		Play::UpdateGameObject(power);
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
			dot.type = TYPE_DESTROYED;
			gState.score += 10;
		}
		Play::UpdateGameObject(dot);
	}
}


// Pacman //
void PacmanTargetReached(GameObject& objPacman)
{
	objPacman.pos = vTiles[pacman.nextTile].pos;
	vTiles[pacman.nextTile].occupied = true;
	pacman.currentTile = pacman.nextTile;
}

void PacmanMainControlls()
{
	if (Play::KeyDown(VK_LEFT) || Play::KeyDown('A'))
		gState.pState = PAC_MOVE_LEFT;

	else if (Play::KeyDown(VK_RIGHT) || Play::KeyDown('D'))
		gState.pState = PAC_MOVE_RIGHT;
	
	else if (Play::KeyDown(VK_UP) || Play::KeyDown('W'))
		gState.pState = PAC_MOVE_UP;
	
	else if (Play::KeyDown(VK_DOWN) || Play::KeyDown('S'))
		gState.pState = PAC_MOVE_DOWN;
}

void SweepNextTile(int id, int oldID)
{
	if (vTiles[id].type != TILE_EMPTY)
		 gState.pState = PAC_IDLE;

	else if (!vTiles[id].occupied)
	{
		pacman.nextTile = id;
		vTiles[oldID].occupied = false;
	}
}


// collisions //
void VulnerableCollision(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);
	GameObject& objPacman = Play::GetGameObjectByType(TYPE_PACMAN);

	if (Play::IsColliding(objGhost, objPacman))
	{
		ghost.state = GHOST_DEAD;
		gState.ghostsEaten++;
		ghost.eaten = true;

		if (gState.ghostsEaten == 1)
			gState.score += 400;
		else if (gState.ghostsEaten == 2)
			gState.score += 800;
		else if (gState.ghostsEaten == 3)
			gState.score += 1200;
		else if (gState.ghostsEaten == 4)
			gState.score += 1600;
	}
}

void ChaseCollision()
{
	GameObject& objPacman = Play::GetGameObjectByType(TYPE_PACMAN);

	for (Ghost& ghost : vGhosts)
	{
		GameObject& objGhost = Play::GetGameObject(ghost.id);

		if (ghost.state == GHOST_SCATTER || ghost.state == GHOST_CHASE)
			if (Play::IsColliding(objGhost, objPacman))
				gState.pState = PAC_DYING;
	}
}


// Ghosts //
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
				if (time > 2.0f)
					ghost.activated = true;
				break;
			}
			case GHOST_INKY:
			{
				if (time > 4.0f)
					ghost.activated = true;
				break;
			}
			case GHOST_CLYDE:
			{
				if (time > 8.0f)
					ghost.activated = true;
				break;
			}
		}
	}
}

void GhostScatterControl()
{
	if (gState.scatter)
		gState.ghostTimer += 1.0f / 60.0f;

	if (Play::RandomRoll(2000) == 1)
	{
		for (Ghost& ghost : vGhosts)
			if (ghost.state == GHOST_CHASE) ghost.state = GHOST_SCATTER;

		gState.ghostTimer = 0.0f;
	}
}

void GhostAI(Ghost& ghost)
{
	switch (ghost.state)
	{
		case GHOST_IDLE:
		{
			SetGhostSprites(ghost);
			if (ghost.activated)
				ExitGhostHouse(ghost);

			break;
		}
		case GHOST_CHASE:
		{
			SetGhostSprites(ghost);
			GhostNextTileReached(ghost);
			SetChaseTarget(ghost);

			if (!gState.vulnerable) ghost.eaten = false;

			if (ghost.nextTile == ghost.currentTile)
				SetGhostDirection(ghost);

			SetGhostNextTile(ghost);
			GhostMovement(ghost);
			
			break;
		}
		case GHOST_SCATTER:
		{
			SetGhostSprites(ghost);
			gState.scatter = true;

			 if (!gState.vulnerable) ghost.eaten = false;

			GhostNextTileReached(ghost);
			SetScatterTarget(ghost);

			if (ghost.nextTile == ghost.currentTile)
				SetGhostDirection(ghost);

			SetGhostNextTile(ghost);
			GhostMovement(ghost);

			if (gState.ghostTimer > 10.0f)
			{
				ReverseDirection(ghost);
				gState.scatter = false;
				ghost.state = GHOST_CHASE;
			}

			break;
		}
		case GHOST_FRIGHTENED:
		{
			SetGhostSprites(ghost);
			VulnerableCollision(ghost);

			if (!gState.vulnerable)
			{
				ReverseDirection(ghost);
				ghost.state = GHOST_CHASE;
				gState.ghostsEaten = 0;
				return;
			}

			GhostNextTileReached(ghost);

			if (ghost.nextTile == ghost.currentTile)
				SetGhostDirection(ghost);

			SetGhostNextTile(ghost);
			GhostMovement(ghost);

			break;
		}
		case GHOST_DEAD:
		{
			Play::SetSprite( Play::GetGameObject(ghost.id), (!ghost.returned) ? SPR_VULNERABLE : ghost.SPRITE, 0.f);

			if (ghost.currentTile == GHOST_EXIT_POS)
			{
				ReturnAndExitGhostHouse(ghost);
				break;
			}

			ghost.targetTile = GHOST_EXIT_POS;

			GhostNextTileReached(ghost);

			if (ghost.nextTile == ghost.currentTile)
				SetGhostDirection(ghost);

			SetGhostNextTile(ghost);
			GhostMovement(ghost);

			break;
		}
	}
}

void SetGhostSprites(Ghost& ghost)
{
	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (ghost.type == GHOST_BLINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_BLINKY_MOVE_UP, 0.1f);
			else if (ghost.type == GHOST_PINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_PINKY_MOVE_UP, 0.1f);
			else if (ghost.type == GHOST_INKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_INKY_MOVE_UP, 0.1f);
			else if (ghost.type == GHOST_CLYDE)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_CLYDE_MOVE_UP, 0.1f);

			break;
		}
		case DIR_DOWN:
		{
			if (ghost.type == GHOST_BLINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_BLINKY_MOVE_DOWN, 0.1f);
			else if (ghost.type == GHOST_PINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_PINKY_MOVE_DOWN, 0.1f);
			else if (ghost.type == GHOST_INKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_INKY_MOVE_DOWN, 0.1f);
			else if (ghost.type == GHOST_CLYDE)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_CLYDE_MOVE_DOWN, 0.1f);
		
			break;
		}
		case DIR_LEFT:
		{
			if (ghost.type == GHOST_BLINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_BLINKY_MOVE_LEFT, 0.1f);
			else if (ghost.type == GHOST_PINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_PINKY_MOVE_LEFT, 0.1f);
			else if (ghost.type == GHOST_INKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_INKY_MOVE_LEFT, 0.1f);
			else if (ghost.type == GHOST_CLYDE)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_CLYDE_MOVE_LEFT, 0.1f);

			break;
		}
		case DIR_RIGHT:
		{
			if (ghost.type == GHOST_BLINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_BLINKY_MOVE_RIGHT, 0.1f);
			else if (ghost.type == GHOST_PINKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_PINKY_MOVE_RIGHT, 0.1f);
			else if (ghost.type == GHOST_INKY)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_INKY_MOVE_RIGHT, 0.1f);
			else if (ghost.type == GHOST_CLYDE)
				Play::SetSprite(Play::GetGameObject(ghost.id), (gState.vulnerable) && ghost.activated && !ghost.eaten ? SPR_VULNERABLE : SPR_CLYDE_MOVE_RIGHT, 0.1f);

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
			ghost.targetTile = pacman.currentTile;
			break;
		}
		case GHOST_CLYDE:
		{
			ghost.targetTile = pacman.currentTile;
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
			ghost.targetTile = BLINKY_SCATTER_POS;
			break;
		}
		case GHOST_PINKY:
		{
			ghost.targetTile = PINKY_SCATTER_POS;
			break;
		}
		case GHOST_INKY:
		{
			ghost.targetTile = INKY_SCATTER_POS;
			break;
		}
		case GHOST_CLYDE:
		{	
			ghost.targetTile = CLYDE_SCATTER_POS;
			break;
		}
	}
}

void SetGhostDirection(Ghost& ghost)
{
	float distLeft = GetDistance(vTiles[ghost.currentTile - 1].pos, vTiles[ghost.targetTile].pos);
	float distRight = GetDistance(vTiles[ghost.currentTile + 1].pos, vTiles[ghost.targetTile].pos);
	float distUp = GetDistance(vTiles[ghost.currentTile - BOARD_SIZE.x].pos, vTiles[ghost.targetTile].pos);
	float distDown = GetDistance(vTiles[ghost.currentTile + BOARD_SIZE.x].pos, vTiles[ghost.targetTile].pos);

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else (distLeft < distRight) ? ghost.dir = DIR_LEFT : ghost.dir = DIR_RIGHT;						
			}
			else if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else (distUp < distRight) ? ghost.dir = DIR_UP : ghost.dir = DIR_RIGHT;					
			}
			else if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else (distUp < distLeft) ? ghost.dir = DIR_UP : ghost.dir = DIR_LEFT;						
			}
			else
			{
				if (distUp <= distLeft && distUp <= distRight) ghost.dir = DIR_UP;
				else if (distLeft <= distUp && distLeft <= distRight) ghost.dir = DIR_LEFT;
				else ghost.dir = DIR_RIGHT;
			}
			break;
		}
		case DIR_DOWN:
		{
			if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else (distLeft < distRight) ? ghost.dir = DIR_LEFT : ghost.dir = DIR_RIGHT;
			}
			else if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else (distDown < distRight) ? ghost.dir = DIR_DOWN : ghost.dir = DIR_RIGHT;
			}
			else if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else (distDown < distLeft) ? ghost.dir = DIR_DOWN : ghost.dir = DIR_LEFT;
			}
			else
			{
				if (distDown <= distLeft && distDown <= distRight) ghost.dir = DIR_DOWN;
				else if (distLeft <= distDown && distLeft <= distRight) ghost.dir = DIR_LEFT;
				else ghost.dir = DIR_RIGHT;
			}
			break;
		}
		case DIR_LEFT:
		{
			if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else (distUp < distDown) ? ghost.dir = DIR_UP : ghost.dir = DIR_DOWN;
			}
			else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else (distLeft < distDown) ? ghost.dir = DIR_LEFT : ghost.dir = DIR_DOWN;
			}
			else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_LEFT;
				else (distLeft < distUp) ? ghost.dir = DIR_LEFT : ghost.dir = DIR_UP;
			}
			else
			{
				if (distLeft <= distUp && distLeft <= distDown) ghost.dir = DIR_LEFT;
				else if (distUp <= distLeft && distUp <= distDown) ghost.dir = DIR_UP;
				else ghost.dir = DIR_DOWN;
			}
			break;
		}
		case DIR_RIGHT:
		{
			if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else (distUp < distDown) ? ghost.dir = DIR_UP : ghost.dir = DIR_DOWN;
			}
			else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_DOWN;
				else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else (distRight < distDown) ? ghost.dir = DIR_RIGHT : ghost.dir = DIR_DOWN;
			}
			else if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
					ghost.dir = DIR_UP;
				else if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
					ghost.dir = DIR_RIGHT;
				else (distRight < distUp) ? ghost.dir = DIR_RIGHT : ghost.dir = DIR_UP;
			}
			else
			{
				if (distRight <= distUp && distRight <= distDown) ghost.dir = DIR_RIGHT;
				else if (distUp <= distRight && distUp <= distDown) ghost.dir = DIR_UP;
				else ghost.dir = DIR_DOWN;
			}
			break;
		}
	}
}

void SetGhostNextTile(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	if (ghost.currentTile == PAC_RESPAWN_POS_RIGHT - 1 && vTiles[ghost.currentTile].ghostOccupied)
	{
		ghost.nextTile = PAC_RESPAWN_POS_RIGHT - 2;
		vTiles[ghost.currentTile].ghostOccupied = false;
		ReverseDirection(ghost);
		return;
	} 
	else if (ghost.currentTile == PAC_RESPAWN_POS_LEFT + 1 && vTiles[ghost.currentTile].ghostOccupied)
	{
		ghost.nextTile = PAC_RESPAWN_POS_LEFT + 2;
		vTiles[ghost.currentTile].ghostOccupied = false;
		ReverseDirection(ghost);
		return;
	}

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (!vTiles[ghost.currentTile - BOARD_SIZE.x].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile - BOARD_SIZE.x;
				vTiles[ghost.currentTile].ghostOccupied = false;
			}
			break;
		}
		case DIR_DOWN:
		{
			if (!vTiles[ghost.currentTile + BOARD_SIZE.x].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile + BOARD_SIZE.x;
				vTiles[ghost.currentTile].ghostOccupied = false;
			}
			break;
		}
		case DIR_LEFT:
		{
			if (!vTiles[ghost.currentTile - 1].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile - 1;
				vTiles[ghost.currentTile].ghostOccupied = false;
			}
			break;
		}
		case DIR_RIGHT:
		{
			if (!vTiles[ghost.currentTile + 1].ghostOccupied)
			{
				ghost.nextTile = ghost.currentTile + 1;
				vTiles[ghost.currentTile].ghostOccupied = false;
			}
			
			break;
		}
	}
}

void GhostMovement(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	(ghost.state == GHOST_DEAD) ? gState.gSpeed = GHOST_DEAD_SPEED : gState.gSpeed = GHOST_SPEED;

	Vector2f vY = { 0.0f, gState.gSpeed };
	Vector2f vX = { gState.gSpeed, 0.0f };

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			objGhost.velocity = vY * (-1);
			break;
		}
		case DIR_DOWN:
		{
			objGhost.velocity = vY;
			break;
		}
		case DIR_LEFT:
		{
			objGhost.velocity = vX * (-1);
			break;
		}
		case DIR_RIGHT:
		{
			objGhost.velocity = vX;
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

			if (objGhost.pos.x < vTiles[GHOST_EXIT_POS].pos.x)
				GhostReadyToGo(ghost, GHOST_EXIT_POS);

			else objGhost.velocity = GHOST_VELOCITY_X * (-1);

			break;
		}
		case GHOST_PINKY:
		{
			GameObject& objGhost = Play::GetGameObject(ghost.id);

			if (objGhost.pos.x > vTiles[GHOST_EXIT_POS + 1].pos.x && objGhost.pos.y < vTiles[INKY_SPAWN_POS].pos.y)
				GhostReadyToGo(ghost, GHOST_EXIT_POS + 1);

			else if (objGhost.pos.y < vTiles[GHOST_EXIT_POS].pos.y)
				objGhost.velocity = GHOST_VELOCITY_X;

			else objGhost.velocity = GHOST_VELOCITY_Y * (-1);

			break;
		}
		case GHOST_INKY:
		{
			GameObject& objGhost = Play::GetGameObject(ghost.id);

			if (objGhost.pos.x > vTiles[GHOST_EXIT_POS + 1].pos.x && objGhost.pos.y < vTiles[INKY_SPAWN_POS].pos.y)
				GhostReadyToGo(ghost, GHOST_EXIT_POS + 1);

			else if (objGhost.pos.y < vTiles[GHOST_EXIT_POS].pos.y)
				objGhost.velocity = GHOST_VELOCITY_X;

			else if (objGhost.pos.x > vTiles[GHOST_EXIT_POS + 1].pos.x - HALF_TILE)
				objGhost.velocity = GHOST_VELOCITY_Y * (-1);
			else
				objGhost.velocity = GHOST_VELOCITY_X;

			break;
		}
		case GHOST_CLYDE:
		{
			GameObject& objGhost = Play::GetGameObject(ghost.id);

			if (objGhost.pos.x < vTiles[GHOST_EXIT_POS].pos.x && objGhost.pos.y < vTiles[CLYDE_SPAWN_POS].pos.y)
				GhostReadyToGo(ghost, GHOST_EXIT_POS);

			else if (objGhost.pos.y < vTiles[GHOST_EXIT_POS].pos.y)
				objGhost.velocity = GHOST_VELOCITY_X * (-1);

			else if (objGhost.pos.x < vTiles[GHOST_EXIT_POS + 1].pos.x - HALF_TILE)
				objGhost.velocity = GHOST_VELOCITY_Y * (-1);
			else
				objGhost.velocity = GHOST_VELOCITY_X * (-1);

			break;
		}
	}
}

void ReturnAndExitGhostHouse(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	if (ghost.returned)
	{
		if (ghost.dir == DIR_LEFT && objGhost.pos.x < vTiles[GHOST_EXIT_POS].pos.x && objGhost.pos.y < vTiles[CLYDE_SPAWN_POS].pos.y
			|| ghost.dir == DIR_RIGHT && objGhost.pos.x > vTiles[GHOST_EXIT_POS + 1].pos.x && objGhost.pos.y < vTiles[CLYDE_SPAWN_POS].pos.y)
		{
			ghost.returned = false;
			ghost.currentTile = (ghost.dir == DIR_RIGHT) ? GHOST_EXIT_POS + 1 : GHOST_EXIT_POS;
			gState.ghostTimer = 0.0f;
			ghost.state = GHOST_SCATTER;
		}

		else if (objGhost.pos.y < vTiles[GHOST_EXIT_POS].pos.y)
			objGhost.velocity = { (ghost.dir == DIR_RIGHT) ? GHOST_DEAD_SPEED : GHOST_DEAD_SPEED * (-1), 0.f };

		else if (objGhost.pos.x < vTiles[GHOST_EXIT_POS + 1].pos.x - HALF_TILE)
			objGhost.velocity = { 0.f, GHOST_DEAD_SPEED * (-1) };
		else
			objGhost.velocity = { GHOST_DEAD_SPEED * (-1), 0.f };
	}
	else
	{
		if (objGhost.pos.x > vTiles[CLYDE_SPAWN_POS].pos.x)
		{
			objGhost.velocity = { 0.f, 0.f };
			ghost.returned = true;
		}

		else if (objGhost.pos.y > vTiles[CLYDE_SPAWN_POS].pos.y)
			objGhost.velocity = { GHOST_DEAD_SPEED, 0.f };
		
		else if (objGhost.pos.x > vTiles[GHOST_EXIT_POS].pos.x + HALF_TILE && objGhost.pos.y < vTiles[CLYDE_SPAWN_POS].pos.y)
			objGhost.velocity = { 0.f, GHOST_DEAD_SPEED };
		
		else objGhost.velocity = { GHOST_DEAD_SPEED, 0.f };
	}
}

void GhostReadyToGo(Ghost& ghost, int pos)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);

	objGhost.pos = vTiles[pos].pos;
	vTiles[GHOST_EXIT_POS].ghostOccupied = true;
	ghost.currentTile = pos;
	ghost.targetTile = pos;
	ghost.nextTile = pos;
	ghost.state = GHOST_SCATTER;
}


// Utility Functions //
float GetDistance(Point2D pos1, Point2D pos2)
{
	float x = pos1.x - pos2.x;
	float y = pos1.y - pos2.y;

	return sqrt(x * x + y * y);
}

void ReverseDirection(Ghost& ghost)
{
	GameObject& objGhost = Play::GetGameObject(ghost.id);
	objGhost.pos = vTiles[ghost.currentTile].pos;

	switch (ghost.dir)
	{
		case DIR_UP:
		{
			if (vTiles[ghost.currentTile + BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY) ghost.dir = DIR_RIGHT;
				else ghost.dir = DIR_LEFT;
			} 
			else ghost.dir = DIR_DOWN;

			break;
		}
		case DIR_DOWN:
		{
			if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY) ghost.dir = DIR_RIGHT;
				else ghost.dir = DIR_LEFT;
			}
			else ghost.dir = DIR_UP;

			break;
		}
		case DIR_LEFT:
		{
			if (vTiles[ghost.currentTile + 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY) ghost.dir = DIR_DOWN;
				else ghost.dir = DIR_UP;
			}
			else ghost.dir = DIR_RIGHT;

			break;
		}
		case DIR_RIGHT:
		{
			if (vTiles[ghost.currentTile - 1].type != TILE_EMPTY)
			{
				if (vTiles[ghost.currentTile - BOARD_SIZE.x].type != TILE_EMPTY) ghost.dir = DIR_DOWN;
				else ghost.dir = DIR_UP;
			}
			else ghost.dir = DIR_LEFT;

			break;
		}
	}
}

void UpdateDestroyed()
{
	std::vector <int> vDestroyed = Play::CollectGameObjectIDsByType(TYPE_DESTROYED);

	for (int id : vDestroyed)
	{
		GameObject& obj = Play::GetGameObject(id);
		Play::DestroyGameObject(id);
	}
}

void RestartGame()
{
	DestroyObjects(TYPE_DOT);
	DestroyObjects(TYPE_POWER);

	vTiles.clear();
	CreateTiles();

	Play::GetGameObjectByType(TYPE_PACMAN).pos = { vTiles[PACMAN_SPAWN_POS].pos.x, vTiles[PACMAN_SPAWN_POS].pos.y };
	Play::GetGameObjectByType(TYPE_PACMAN).frame = 0;
	Play::GetGameObjectByType(TYPE_PACMAN).rotation = 0.0f;
	Play::SetSprite(Play::GetGameObjectByType(TYPE_PACMAN), SPR_PACMAN, 0.f);
	pacman.currentTile = PACMAN_SPAWN_POS;
	pacman.nextTile = PACMAN_SPAWN_POS;

	RestartGhosts();

	gState.ghostTimer = 0.0f;
	gState.powerTimer = 0.0f;
	gState.time = 0.0f;
	gState.score = 0;
	gState.scatter = false;
	gState.vulnerable = false;
	gState.maxDots = 0;

	gState.gSpeed = GHOST_SPEED;
	gState.pSpeed = PACMAN_SPEED;

	gState.state = STATE_PLAY;
	gState.pState = PAC_IDLE;

	gState.gameRestarted = true;
}

void RestartGhosts()
{
	for (Ghost& ghost : vGhosts)
	{
		ghost.state = GHOST_IDLE;
		ghost.activated = false;
		ghost.returned = false;
		ghost.exited = false;

		GameObject& objGhost = Play::GetGameObject(ghost.id);
		objGhost.velocity = { 0, 0 };

		switch (ghost.type)
		{
			case GHOST_BLINKY:
			{
				objGhost.pos = { vTiles[BLINKY_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[BLINKY_SPAWN_POS].pos.y };
				ghost.dir = DIR_LEFT;
				break;
			}
			case GHOST_PINKY:
			{
				objGhost.pos = { vTiles[PINKY_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[PINKY_SPAWN_POS].pos.y };
				ghost.dir = DIR_UP;
				break;
			}
			case GHOST_INKY:
			{
				objGhost.pos = { vTiles[INKY_SPAWN_POS].pos.x - (TILE_SIZE / 2), vTiles[INKY_SPAWN_POS].pos.y };
				ghost.dir = DIR_RIGHT;
				break;
			}
			case GHOST_CLYDE:
			{
				objGhost.pos = { vTiles[CLYDE_SPAWN_POS].pos.x + (TILE_SIZE / 2), vTiles[CLYDE_SPAWN_POS].pos.y };
				ghost.dir = DIR_LEFT;
				break;
			}
		}
	}
}

void DestroyObjects(int TYPE)
{
	std::vector <int> vObjects = Play::CollectGameObjectIDsByType(TYPE);
	for (int id : vObjects)
		Play::DestroyGameObject(id);
}
