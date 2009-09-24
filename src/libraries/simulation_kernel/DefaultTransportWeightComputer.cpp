/*
 * DefaultTransportWeightComputer.cpp
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "DefaultTransportWeightComputer.h"

namespace transport
{

DefaultTransportWeightComputer::DefaultTransportWeightComputer() :
  totalTransportedWeight_(0.),
  heaviestTransportedWeight_(0.),
  strategy_(0)
{

}

DefaultTransportWeightComputer::~DefaultTransportWeightComputer()
{

}

void DefaultTransportWeightComputer::Reset(const TransportStrategy_ABC* st)
{
  totalTransportedWeight_ = heaviestTransportedWeight_ = 0.;
  strategy_ = st;
}

void DefaultTransportWeightComputer::AddTransportedWeight(double weight, bool canBeLoaded)
{
	if(!strategy_ || (strategy_ && strategy_->Autorize(canBeLoaded) ) )
	{
		totalTransportedWeight_ += weight;
		heaviestTransportedWeight_ = std::max(heaviestTransportedWeight_, weight);
	}
}

double DefaultTransportWeightComputer::TotalTransportedWeight() const
{
  return totalTransportedWeight_;
}

double DefaultTransportWeightComputer::HeaviestTransportedWeight() const
{
 return  heaviestTransportedWeight_;
}

}
