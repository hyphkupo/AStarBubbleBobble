#pragma once

#include "Actor/DrawableActor.h"
#include "Level/GameLevel.h"

class Goal : public DrawableActor
{
	RTTI_DECLARATIONS(Goal, DrawableActor)

public:
	Goal(GameLevel* refLevel);

	virtual void Update(float deltaTime) override;

private:
	GameLevel* refLevel = nullptr;

};