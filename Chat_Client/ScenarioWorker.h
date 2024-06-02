#pragma once
#include "Worker.h"

class ServerSession;
class ScenarioWorker : public Worker
{
public:
    ScenarioWorker();
    ~ScenarioWorker() override;

public:
    void Init() override;
    void Work() override;
	void ExecuteScenario(int scenarioId);

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
	std::vector<int> _endScenario;

	bool endFlag;
};
