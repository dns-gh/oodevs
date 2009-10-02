/*
 * DefaultSpeedComputer.cpp
 *
 *  Created on: 1 oct. 2009
 *      Author: ahc
 */
#include "simulation_kernel_pch.h"

#include "DefaultSpeedComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

DefaultSpeedComputer::DefaultSpeedComputer() :
		strategy_(0),
		speed_(std::numeric_limits<double>::max()),
		hasUsableComponent_(false)
{
}

DefaultSpeedComputer::~DefaultSpeedComputer()
{
}

void DefaultSpeedComputer::Reset(const SpeedStrategy_ABC* strategy)
{
	strategy_ = strategy;
	speed_ = std::numeric_limits<double>::max();
	hasUsableComponent_ = false;
	speedRatio_ = 1;

}

void DefaultSpeedComputer::ApplyOnComponent(const PHY_ComposantePion& component)
{
	assert( strategy_);
	if(component.CanMove())
	{
		speed_ = std::min(speed_ , strategy_->ApplyOnComponent(component));
		hasUsableComponent_ = true;
	}
}

void DefaultSpeedComputer::ApplyOnReinforcement( MIL_AgentPion& reinforcement)
{
	assert( strategy_);
	speed_ = std::min(speed_ , strategy_->ApplyOnReinforcement(reinforcement));
}

void DefaultSpeedComputer::ApplyOnPopulation( const DEC_Knowledge_PopulationCollision& population)
{
	assert( strategy_);
	speed_ = std::min(speed_ , strategy_->ApplyOnPopulation(population));
}


double DefaultSpeedComputer::GetSpeed() const
{
	return hasUsableComponent_ ? speedRatio_ * speed_ : 0;
}

void DefaultSpeedComputer::AddModifier(double ratio, bool isMax)
{
	speedRatio_ *= strategy_->AddModifier(ratio, isMax);
}

}
