#pragma once

#include "Actor/DrawableActor.h"
#include <vector>

class GameLevel;
class Node;
class Start;
class Goal;
class AStar;
class Player;
class Level;
class Enemy : public DrawableActor
{
	// 적 캐릭터가 이동할 방향.
	enum class Direction
	{
		None = -1,
		Left,
		Right
	};

	RTTI_DECLARATIONS(Enemy, DrawableActor)

public:
	bool inBubble = false;

	Start* s = nullptr;
	Goal* g = nullptr;

	Node* startNode = nullptr;
	Node* goalNode = nullptr;

	std::vector<Vector2> pathNode;

public:
	Enemy(const char* image, int x, int y, int inputPositionY, GameLevel* level);

	virtual void Update(float deltaTime) override;

	virtual void SetPosition(const Vector2& newPosition) override;
	
	void SetPositionY() { yPosition = position.y; };

private:
	int minX;
	int maxX;

	Direction direction = Direction::None;
	float xPosition = 0.0f;
	float yPosition = 0.0f;

	float speed = 2.5f;
	float factor = 0.0f;
	float AccTimeDelta = 0.f;

	GameLevel* refLevel = nullptr;

private:
	void MoveEnemy(float deltaTime);

	void MoveInBubble(float deltaTime);

};