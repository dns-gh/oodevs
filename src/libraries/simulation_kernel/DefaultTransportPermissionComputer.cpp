/*
 * TransportPermissionComputer.cpp
 *
 *  Created on: 25 sept. 2009
 *      Author: ahc
 */
#include "simulation_kernel_pch.h"
#include "DefaultTransportPermissionComputer.h"

namespace transport
{

DefaultTransportPermissionComputer::DefaultTransportPermissionComputer() :
	allow_(true)
{
}

DefaultTransportPermissionComputer::~DefaultTransportPermissionComputer()
{
	// TODO Auto-generated destructor stub
}

void DefaultTransportPermissionComputer::Reset()
{
	allow_ = false;
}

void DefaultTransportPermissionComputer::AllowLoading(bool doAllow)
{
	allow_ &= doAllow;
}
bool DefaultTransportPermissionComputer::CanBeLoaded() const
{
	return allow_;
}

}
