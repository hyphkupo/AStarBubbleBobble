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

	// 플레이어가 이동이 가능한 지 확인하는 함수
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
	// 스테이지 파일 로드 함수
	void LoadNextStage();

	// 플레이어 버블과 적의 충돌 처리
	void ProcessCollisionPlayerBubbleAndEnemy();

	// 적과 플레이어의 충돌 처리
	void ProcessCollisionPlayerAndEnemy();

	// 게임을 클리어했는지 확인하는 함수
	bool CheckGameClear();

	//void AStarPath(Enemy* e, Player* p, std::vector<std::vector<char>> aStarMap, float deltaTime);

private:
	// 벽/땅 액터 배열
	List<DrawableActor*> map;

	// 점수
	int score = 0;

	// 플레이어 액터
	Player* player = nullptr;

	// 게임 클리어 변수
	bool isGameClear = false;
	int gameClearNumber = 0;

	float speed = 3.0f;
	float AccTimeDelta = 0.f;

};