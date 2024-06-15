#pragma once
#include "Worker.h"

class ServerSession;
class Scenario
{
public:
    Scenario();
    ~Scenario();

public:
    void Init();

	void ExecuteScenario(int scenarioId);

	void StartScenario();
	void NextScenario();
	void ScenarioEnd();
public:
	void EnterMap();
	void Chat();
	void ExitMap();
	void Move();
	void Attack();

public:

	void SetOwner(std::shared_ptr<ServerSession> session) { _owner = session; }
	std::shared_ptr<ServerSession> GetOwner() {return _owner; }
private:
	std::shared_ptr<ServerSession> _owner;
    std::vector<int> _beginScenario;
	std::vector<int> _repeatScenario;
	int currentScenarioIdx;
	std::vector<int> _endScenario;
};
