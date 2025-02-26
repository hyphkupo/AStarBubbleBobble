#include "GameLevel.h"
#include "Engine/Engine.h"
#include "Level/Level.h"

#include "Actor/Wall.h"
#include "Actor/Ground.h"
#include "Actor/Air.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBubble.h"

#include "Actor/Start.h"
#include "Actor/Goal.h"
#include "Actor/Path.h"

#include "Algorithm/AStar.h"
#include "Algorithm/Node.h"

#include <Windows.h>
#include "Game/Game.h"

GameLevel::GameLevel()
{
	LoadNextStage();
}

void GameLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	//AStarPath(e1, p, aStarMap, deltaTime);
	//AStarPath(e2, p, aStarMap, deltaTime);
	
	// �÷��̾� ����� ���� �浹 ó��.
	ProcessCollisionPlayerBubbleAndEnemy();

	// ���� �÷��̾��� �浹 ó��.
	ProcessCollisionPlayerAndEnemy();

	if (isPlayerDead)
	{
		AccTimeDelta += deltaTime;
		if (AccTimeDelta > 2.0f)
		{
			AccTimeDelta -= 2.0f;

			Engine::Get().QuitGame();
		}
	}

	else if (CheckGameClear())
	{
		AccTimeDelta += deltaTime;
		if (AccTimeDelta > 2.0f)
		{
			AccTimeDelta -= 2.0f;

			++gameClearNumber;
			if (gameClearNumber == 3)
			{
				Engine::Get().QuitGame();
				return;
			}

			// map ������ ����
			for (int ix = 0; ix < map.Size();)
			{
				map.Erase(ix);
			}

			if (stageNumber == 3)
			{
				p->Destroy();
				e1->Destroy();
			}

			else
			{
				p->Destroy();
				e1->Destroy();
				e2->Destroy();
			}

			LoadNextStage();
		}
	}
}

void GameLevel::Draw()
{
	Super::Draw();

	// ���� ���.
	char buffer[256];
	snprintf(buffer, 256, "Score: %d", score);
	Engine::Get().Draw(Vector2(0, Engine::Get().ScreenSize().y - 1), buffer);

	if (isPlayerDead)
	{
		int y = Engine::Get().ScreenSize().y;
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x - 10, Engine::Get().ScreenSize().y - 1), "Game Over!");
		//Engine::Get().Present();

		//Sleep(3000);
	}

	else if (CheckGameClear())
	{
		int y = Engine::Get().ScreenSize().y;
		Engine::Get().Draw(Vector2(Engine::Get().ScreenSize().x - 11, Engine::Get().ScreenSize().y - 1), "Game Clear!");
		//Engine::Get().Present();

		//Sleep(3000);

		//Engine::Get().QuitGame();
	}
}

bool GameLevel::isGround(const Vector2& position)
{
	// �̵��Ϸ��� ��ġ�� ���� �ִ��� Ȯ��
	DrawableActor* searchedActor = nullptr;

	// �̵��Ϸ��� ��ġ�� ���� ã��
	for (auto* actor : map)
	{
		if (actor->Position() == position)
		{
			searchedActor = actor;
			break;
		}
	}

	// �˻��� ���Ͱ� �� �������� Ȯ��
	if (searchedActor->As<Ground>())
	{
		return true;
	}

	return false;
}

void GameLevel::LoadNextStage()
{
	// Ŀ�� ���߱�.
	Engine::Get().SetCursorType(CursorType::NoCursor);

	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			aStarMap.emplace_back('0');
		}
	}

	//aStarMap(10, std::vector<char> (10, 0));

	// �� ���� �ҷ��� ���� �ε�.
	// ���� �б�.
	FILE* file = nullptr;

	char stageFileName[26] = "../Assets/Maps/Stage";
	char stageCharNumber[2];
	_itoa_s(stageNumber, stageCharNumber, 10);
	strcat_s(stageFileName, 26, stageCharNumber);
	strcat_s(stageFileName, 26, ".txt");	

	//fopen_s(&file, "../Assets/Maps/Stage1.txt", "rb");
	fopen_s(&file, stageFileName, "rb");

	// ���� ó��.
	if (file == nullptr)
	{
		std::cout << "�� ���� ���� ����.\n";
		__debugbreak();
		return;
	}

	// ���� �б�.
	// ����ġ�� �̵�.
	fseek(file, 0, SEEK_END);

	// �̵��� ��ġ�� FP ��������.
	size_t readSize = ftell(file);

	// FP ����ġ.
	rewind(file);

	// ���� �о ���ۿ� ���.
	char* buffer = new char[readSize + 1];
	size_t bytesRead = fread(buffer, 1, readSize, file);

	if (readSize != bytesRead)
	{
		std::cout << "�о�� ũ�Ⱑ �ٸ�\n";
		__debugbreak();
		return;
	}

	buffer[readSize] = '\0';

	// ���� ���� �� ����� �ε���.
	int index = 0;

	// ��ǥ ����� ���� ���� ����.
	int xPosition = 0;
	int yPosition = 0;

	// �ؼ� (�Ľ�-Parcing).
	while (index < (int)bytesRead)
	{
		// �� ���ھ� �б�.
		char mapChar = buffer[index++];

		// ���� ������ ��� ó��.
		if (mapChar == '\n')
		{
			++yPosition;
			xPosition = 0;
			continue;
		}

		// �� ���ڰ� 1�̸� Wall ���� ����
		if (mapChar == '1')
		{
			Wall* wall = new Wall(Vector2(xPosition, yPosition));
			actors.PushBack(wall);
			map.PushBack(wall);
			aStarMap[yPosition][xPosition] = '1';
			//aStarMap.emplace_back('1');
		}

		// �� ���ڰ� 0�̸� Ground ���� ����
		else if (mapChar == '0')
		{
			Ground* ground = new Ground(Vector2(xPosition, yPosition));
			actors.PushBack(ground);
			map.PushBack(ground);
			aStarMap[yPosition][xPosition] = '0';
			//aStarMap.emplace_back('0');
		}

		// �� ���ڰ� .�̸� Air ���� ����
		else if (mapChar == '.')
		{
			Air* air = new Air(Vector2(xPosition, yPosition));
			actors.PushBack(air);
			map.PushBack(air);
			aStarMap[yPosition][xPosition] = '.';
			//aStarMap.emplace_back('.');
		}

		++xPosition;
	}

	if (stageNumber == 1)
	{
		p = new Player("p", this);
		e1 = new Enemy("x", 9, 22, 4, this);
		e2 = new Enemy("x", 9, 22, 7, this);

		AddActor(p);
		AddActor(e1);
		AddActor(e2);
	}
	
	else if (stageNumber == 2)
	{
		p = new Player("p", this);
		e1 = new Enemy("x", 10, 14, 3, this);
		e2 = new Enemy("x", 17, 21, 3, this);

		AddActor(p);
		AddActor(e1);
		AddActor(e2);
	}

	else if (stageNumber == 3)
	{
		p = new Player("p", this);
		e1 = new Enemy("b", 1, 9, 16, this);

		AddActor(p);
		AddActor(e1);
	}

	delete[] buffer;

	fclose(file);

	++stageNumber;
}

// �÷��̾��� ����� �� �浹 ó��
void GameLevel::ProcessCollisionPlayerBubbleAndEnemy()
{
	// ���� �� �� ĳ���� �迭 ����
	List<PlayerBubble*> bubbles;
	List<Enemy*> enemies;
	Player* player = nullptr;

	// ������ ��ġ�� ���͸� ��ȸ�ϸ鼭 ����Ʈ ä���
	for (Actor* actor : actors)
	{
		// �÷��̾ �������� �ʾ����� �÷��̾�� ����ȯ
		if (!player)
		{
			player = actor->As<Player>();
			continue;
		}

		// ����ȯ �� Ȯ���ؼ� ����Ʈ ä���
		PlayerBubble* bubble = actor->As<PlayerBubble>();
		if (bubble)
		{
			bubbles.PushBack(bubble);
			continue;
		}

		// ������ ����ȯ �� Ȯ���ؼ� ����Ʈ ä���
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// ���� ó��
	if (bubbles.Size() == 0 || enemies.Size() == 0)
	{
		return;
	}

	// �� �迭�� ��ȸ�ϸ鼭 �浹 ó��
	for (PlayerBubble* bubble : bubbles)
	{
		for (Enemy* enemy : enemies)
		{
			// ���� ��Ȱ��ȭ ���¶�� �ǳʶٱ�
			if (!enemy->IsActive())
			{
				continue;
			}

			// �浹 ó��
			if (enemy->Intersect(*bubble))
			{
				enemy->inBubble = true;
				enemy->SetPositionY();

				if (enemy->inBubble)
				{
					if (stageNumber == 4)
					{
						enemy->RedrawImage("&", Color::Yellow);
					}

					else
					{
						enemy->RedrawImage("@", Color::Yellow);
					}
				}

				// �� ������ ����� �� ����
				if (enemy->Position().y < 1)
				{
					enemy->Destroy();
				}

				// ���� ����
				bubble->Destroy();
			}
		}
	}
}

// �÷��̾�� �� �浹 ó��
void GameLevel::ProcessCollisionPlayerAndEnemy()
{
	// ź�� �� �� ĳ���� �迭 ����.
	List<Enemy*> enemies;
	Player* player = nullptr;

	// ���� ����� ��ȸ�ϸ鼭 ź��� �� ĳ���͸� ã�� �迭�� �߰�.
	for (Actor* actor : actors)
	{
		// �÷��̾ �������� �ʾ����� �÷��̾�� ����ȯ.
		if (!player)
		{
			player = actor->As<Player>();
			continue;
		}

		// ������ ����ȯ �����ϸ� ��Ͽ� �߰�.
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// �÷��̾ ��ã�Ұų� �� ź���� ������ �浹 ó������ �ʰ� ����.
	if (player == nullptr || enemies.Size() == 0)
	{
		return;
	}

	// �� �迭�� ��ȸ�ϸ鼭 �浹 ó��.
	for (Enemy* enemy : enemies)
	{
		// �÷��̾ ��Ȱ��ȭ �����̸� �ǳʶٱ�.
		if (!player->IsActive())
		{
			continue;
		}

		// �÷��̾�� ���� �浹�ߴ��� Ȯ��
		if (player->Intersect(*enemy) && !enemy->inBubble)
		{
			isPlayerDead = true;
			//playerDeadPosition = Vector2(player->Position().x + player->Width() / 2, player->Position().y);
			player->Destroy();
		}

		// ���� �÷��̾�� ������ ���� �� �÷��̾�� �ε����ٸ� ���� �߰�, �� ����
		if (player->Intersect(*enemy) && enemy->inBubble)
		{
			if (stageNumber == 4)
			{
				score += 300;
			}

			else
			{
				score += 100;
			}

			enemy->Destroy();
		}
	}
}

bool GameLevel::CheckGameClear()
{
	List<Enemy*> enemies;

	for (Actor* actor : actors)
	{
		// ������ ����ȯ �����ϸ� ��Ͽ� �߰�.
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			return false;
		}
	}

	return true;
}

//void GameLevel::AStarPath(Enemy* e, Player* p, std::vector<std::vector<char>> aStarMap, float deltaTime)
//{
//	pathNode.clear();
//
//	Path* pa = nullptr;
//
//	AccTimeDelta += deltaTime * speed;
//	if (AccTimeDelta > 1.f)
//	{
//		AccTimeDelta -= 1.f;
//		// TODO �̵�
//		startNode = new Node(e->Position());
//		goalNode = new Node(p->Position());
//
//		AStar aStar;
//		std::vector<Node*> path = aStar.FindPath(startNode, goalNode, aStarMap);
//
//		
//		if (!path.empty())
//		{
//			for (Actor* a : actors)
//			{
//				for (Node* node : path)
//				{
//					if (node->position.x == a->Position().x && node->position.y && a->Position().y)
//					{
//						pathNode.push_back(Vector2(node->position.x, node->position.y));
//						hasPathNode = true;
//					}
//				}
//			}
//		}
//		
//
//		if (!path.empty())
//		{
//			/*for (Node* node : path)
//			{
//				pathNode.push_back(Vector2(node->position.x, node->position.y));
//			}*/
//			e->SetPosition(path[1]->position);
//		}
//
//		//if (position.x == a.x && position.y == a.y) break;		// �浹 �� ���� ������ �ǵ��� �ּ� ó��
//
//		//int ax = 0;
//		//int ay = 0;
//		//float deltaTimeSecond = 0.0f;
//
//		//xPosition = path[0]->position.x;
//		//yPosition = path[0]->position.y;
//		//++ax;
//		//++ay;
//
//		SafeDelete(goalNode);
//	}
//}

bool GameLevel::CanPlayerMove(const Vector2& position)
{
	// ������ Ŭ����� ��� �ٷ� ����.
	if (isGameClear)
	{
		return false;
	}

	// �̵��Ϸ��� ��ġ�� ���� �ִ��� Ȯ��.
	DrawableActor* searchedActor = nullptr;

	// ���� �̵��Ϸ��� ��ġ�� ���� ã��.
	for (auto* actor : map)
	{
		if (actor->Position() == position)
		{
			searchedActor = actor;
			break;
		}
	}

	// �˻��� ���Ͱ� ������ Ȯ��.
	if (searchedActor->As<Wall>())
	{
		return false;
	}

	// �˻��� ���Ͱ� �̵� ������ ����(��/Ÿ��)���� Ȯ��.
	if (searchedActor->As<Ground>())
	{
		return false;
	}

	return true;
}

