#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

#include "Play.h"
#include "MainGame.h"



// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	Play::CentreAllSpriteOrigins();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Draw();

	return Play::KeyDown( VK_ESCAPE );
}


// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}

void Draw()
{
	Play::ClearDrawingBuffer(Play::cBlack);

	Play::DrawSprite(SPR_BACKGROUND, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 1);


	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 31; j++)
		{
			Point2D tilePos = { 370 + i * TILE_SIZE, 70 + j * TILE_SIZE };
			Play::DrawRect({ tilePos.x - TILE_SIZE / 2, tilePos.y - TILE_SIZE / 2 }, { tilePos.x + TILE_SIZE / 2, tilePos.y + TILE_SIZE / 2 }, Play::cWhite);
		}
	}

	Play::PresentDrawingBuffer();
}







