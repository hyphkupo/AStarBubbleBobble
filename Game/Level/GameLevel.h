#pragma once

#include <Level/Level.h>
#include <Math/Vector2.h>
#include <vector>

class DrawableActor;
class Player;
class Enemy;
class Node;
class Start;
class Goal;
class AStar;
class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	bool isGround(const Vector2& position);

	// �÷��̾ �̵��� ������ �� Ȯ���ϴ� �Լ�
	bool CanPlayerMove(const Vector2& position);

public:
	bool isPlayerDead = false;

	int stageNumber = 1;

	std::vector <std::vector<char>> aStarMap;

	Player* p = nullptr;
	Enemy* e1 = nullptr;
	Enemy* e2 = nullptr;

	//Node* startNode = nullptr;
	//Node* goalNode = nullptr;

	//std::vector<Vector2> pathNode;

private:
	// �������� ���� �ε� �Լ�
	void LoadNextStage();

	// �÷��̾� ����� ���� �浹 ó��
	void ProcessCollisionPlayerBubbleAndEnemy();

	// ���� �÷��̾��� �浹 ó��
	void ProcessCollisionPlayerAndEnemy();

	// ������ Ŭ�����ߴ��� Ȯ���ϴ� �Լ�
	bool CheckGameClear();

	//void AStarPath(Enemy* e, Player* p, std::vector<std::vector<char>> aStarMap, float deltaTime);

private:
	// ��/�� ���� �迭
	List<DrawableActor*> map;

	// ����
	int score = 0;

	// �÷��̾� ����
	Player* player = nullptr;

	// ���� Ŭ���� ����
	bool isGameClear = false;
	int gameClearNumber = 0;

	float speed = 3.0f;
	float AccTimeDelta = 0.f;

};