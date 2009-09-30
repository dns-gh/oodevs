/*
 * LoadingComputerFactory.cpp
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"
#include "LoadingComputerFactory.h"
#include "DefaultHumanLoadingTimeComputer.h"
#include "DefaultLoadedStateConsistencyComputer.h"

namespace transport
{

LoadingComputerFactory::LoadingComputerFactory() :
	humanLoadingTimeComputer_(new DefaultHumanLoadingTimeComputer()),
	loadedStateConsistencyComputer_(new DefaultLoadedStateConsistencyComputer())
{
}

LoadingComputerFactory::~LoadingComputerFactory()
{
}

HumanLoadingTimeComputer_ABC& LoadingComputerFactory::CreateHumanLoadingTimeComputer()
{
	humanLoadingTimeComputer_->Reset();
	return *humanLoadingTimeComputer_;
}
LoadedStateConsistencyComputer_ABC& LoadingComputerFactory::CreateLoadedStateConsistencyComputer()
{
	loadedStateConsistencyComputer_->Reset();
	return *loadedStateConsistencyComputer_;
}

}
