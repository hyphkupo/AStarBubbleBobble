#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Start : public DrawableActor
{
	RTTI_DECLARATIONS(Start, DrawableActor)

public:
	Start(GameLevel* refLevel);

	virtual void Update(float deltaTime) override;

private:
	GameLevel* refLevel = nullptr;
};