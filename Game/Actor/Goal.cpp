#include "Goal.h"
#include "Engine/Engine.h"

Goal::Goal(GameLevel* refLevel)
	: DrawableActor("e"), refLevel(refLevel)
{
	color = Color::Green;
	//position = Vector2(5, 5);
}

void Goal::Update(float deltaTime)
{
	DrawableActor::Update(deltaTime);

	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}
}