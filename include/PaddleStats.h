#pragma once
#include "Types.h"
#include <unordered_set>
#include <unordered_map>

struct PaddleStats
{
	std::unordered_set<UpgradeType> uniqueUpgrades;
	std::unordered_map<UpgradeType, int> stackableUpgrades;

	// Dash
	float dashTimer = 0.1f;
	float dashCooldown = 0.f;
	float dashSpeedMultiplier = 1.f;
	bool hasDashUpgrade = false;
	bool isDashing = false;

	// Movement
	bool canMoveUpAndDown = false;

	// Energy
	float energyRangeModifier = 0.f;
	bool isNeverExhausted = false;

	// Ball
	float spinMultiplier = 1.0f;
	float curvaturePower = 0.f;

	// Foresight
	bool hasForesight = false;

	void Reset()
	{
		uniqueUpgrades.clear();
		stackableUpgrades.clear();

		float dashTimer = 0.1f;
		float dashCooldown = 0.f;
		float dashSpeedMultiplier = 1.f;
		bool hasDashUpgrade = false;
		bool isDashing = false;

		bool canMoveUpAndDown = false;

		float energyRangeModifier = 0.f;
		bool isNeverExhausted = false;

		float spinMultiplier = 1.0f;
		float curvaturePower = 0.f;

		bool hasForesight = false;
	}
};