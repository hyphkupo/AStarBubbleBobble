#include "Enemy.h"
#include "Engine/Engine.h"
#include "Math/Vector2.h"
#include "Level/Level.h"
#include "Level/GameLevel.h"

#include "Actor/DrawableActor.h"
#include "Actor/Player.h"
#include "Actor/Start.h"
#include "Actor/Goal.h"
#include "Actor/Path.h"

#include "Algorithm/AStar.h"
#include "Algorithm/Node.h"

Enemy::Enemy(const char* image, int x, int y, int inputPositionY, GameLevel* level)
	: DrawableActor("a"), minX(x), maxX(y), refLevel(level)
{
	if (refLevel->stageNumber == 1)
	{
		// �� ���� ��ġ
		xPosition = Random(minX, maxX);
		position = Vector2(xPosition, inputPositionY);
		yPosition = (float)position.y;
	}

	else if (refLevel->stageNumber == 2)
	{
		// �� ���� ��ġ
		xPosition = Random(minX, maxX);
		position = Vector2(xPosition, inputPositionY);
		yPosition = (float)position.y;
	}

	else if (refLevel->stageNumber == 3)
	{
		// �� ���� ��ġ
		speed = 8.0f;
		RedrawImage("*", Color::White);
		yPosition = Random(minX, maxX);
		position = Vector2(inputPositionY, yPosition);
		xPosition = (float)position.x;
	}
}

void Enemy::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// ���� ������ ���� ���¶�� �� --y
	if (inBubble)
	{
		MoveInBubble(deltaTime);
	}

	// ���� ������ ���� ���� ����
	else
	{
		MoveEnemy(deltaTime);
	}
}

void Enemy::SetPosition(const Vector2& newPosition)
{
	Super::SetPosition(newPosition);
}

void Enemy::MoveEnemy(float deltaTime)
{
	/*
	//// Stage3
	//if (refLevel->stageNumber == 4)
	//{
	//	// ���� �̵�
	//	yPosition += 10.0f * factor * deltaTime;
	//	position.y = (int)yPosition;

	//	// ���� ����� �ݴ� �������� �̵�
	//	if (position.y < minX || position.y > maxX)
	//	{
	//		factor *= (-1.0f);
	//		yPosition += speed * factor * deltaTime;
	//		position.y = (int)yPosition;
	//	}
	//}
	//
	//else
	//{
	//	// �¿� �̵�
	//	xPosition += speed * factor * deltaTime;
	//	position.x = (int)xPosition;

	//	// ���� ����� �ݴ� �������� �̵�
	//	if (position.x < minX || position.x > maxX)
	//	{
	//		factor *= (-1.0f);
	//		xPosition += speed * factor * deltaTime;
	//		position.x = (int)xPosition;
	//	}
	//}
	*/

	// ���� ó��.
	if (deltaTime > 1.0f)
	{
		return;
	}
	
	pathNode.clear();
	
	Path* pa = nullptr;
	
	AccTimeDelta += deltaTime * speed;
	if (AccTimeDelta > 1.f)
	{
		AccTimeDelta -= 1.f;
		// TODO �̵�
		startNode = new Node(Position());
		goalNode = new Node(refLevel->p->Position());
	
		AStar aStar;
		std::vector<Node*> path = aStar.FindPath(startNode, goalNode, refLevel->aStarMap);
	
		//if (!path.empty())
		//{
		//	for (Actor* a : actors)
		//	{
		//		for (Node* node : path)
		//		{
		//			if (node->position.x == a->Position().x && node->position.y && a->Position().y)
		//			{
		//				pathNode.push_back(Vector2(node->position.x, node->position.y));
		//				hasPathNode = true;
		//			}
		//		}
		//	}
		//}
	
		if (!path.empty())
		{
			/*for (Node* node : path)
			{
				pathNode.push_back(Vector2(node->position.x, node->position.y));
			}*/
			this->SetPosition(path[1]->position);
		}
	
		/*if (position.x == a.x && position.y == a.y) break;*/		// �浹 �� ���� ������ �ǵ��� �ּ� ó��
	
		//int ax = 0;
		//int ay = 0;
		//float deltaTimeSecond = 0.0f;
	
		//xPosition = path[0]->position.x;
		//yPosition = path[0]->position.y;
		//++ax;
		//++ay;
	
		SafeDelete(goalNode);
	}
	//for (auto& a : refLevel->pathNode)
	//{
	//	xPosition = a.x;
	//	yPosition = a.y;
	//	position.x = (int)xPosition;
	//	position.y = (int)yPosition;
	//}
}

// ���� ������ ���� ���¶�� �� --y
void Enemy::MoveInBubble(float deltaTime)
{
	yPosition -= speed / 2 * deltaTime;
	position.y = (int)yPosition;

	if (position.y < 0)
	{
		Destroy();
		return;
	}
}