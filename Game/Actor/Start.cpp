#include "Start.h"
#include "Engine/Engine.h"

Start::Start(GameLevel* refLevel)
	: DrawableActor("s"), refLevel(refLevel)
{
	color = Color::Red;
}

void Start::Update(float deltaTime)
{
	Super::Update(deltaTime);
}