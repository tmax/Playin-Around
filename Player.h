#pragma once

#include "Agent.h"

class Agent;

class Player
{
private:
	Agent* player;

public:
	Player(Agent* agent){player = agent;};
};