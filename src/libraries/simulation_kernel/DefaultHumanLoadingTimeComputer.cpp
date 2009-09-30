/*
 * DefaultHumanLoadingTimeComputer.cpp
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"
#include "DefaultHumanLoadingTimeComputer.h"

namespace transport
{

DefaultHumanLoadingTimeComputer::DefaultHumanLoadingTimeComputer() :
	humansCount_(0),
	humansLoadedPerTimeStep_(0.),
	humansUnloadedPerTimeStep_(0.)
{

}

DefaultHumanLoadingTimeComputer::~DefaultHumanLoadingTimeComputer()
{

}

void DefaultHumanLoadingTimeComputer::AddHumans(int humansCount)
{
	humansCount_ += humansCount;
}

void DefaultHumanLoadingTimeComputer::AddTime(double loadingPerTimeStep, double unloadingPerTimeStep)
{
	humansLoadedPerTimeStep_+=loadingPerTimeStep;
	humansUnloadedPerTimeStep_+=unloadingPerTimeStep;
}

void DefaultHumanLoadingTimeComputer::Reset()
{
	humansCount_ = 0;
	humansLoadedPerTimeStep_ = humansUnloadedPerTimeStep_ = 0.;
}
int DefaultHumanLoadingTimeComputer::GetHumansCount() const
{
	return humansCount_;
}
double DefaultHumanLoadingTimeComputer::GetHumansLoadedPerTimeStep() const
{
	return humansLoadedPerTimeStep_;
}
double DefaultHumanLoadingTimeComputer::GetHumansUnloadedPerTimeStep() const
{
	return humansUnloadedPerTimeStep_;
}
}
