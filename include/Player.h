#pragma once
#include "Paddle.h"

class Player : public Paddle
{
public:
	void Update(const float& deltaT) override;
};