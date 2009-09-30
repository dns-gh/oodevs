/*
 * DefaultLoadedStateConsistencyComputer.cpp
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */
#include "simulation_kernel_pch.h"
#include "DefaultLoadedStateConsistencyComputer.h"

namespace transport
{

DefaultLoadedStateConsistencyComputer::DefaultLoadedStateConsistencyComputer() :
	hasValidCarrier_(false),
	hasValidLoadable_(false)
{
}

DefaultLoadedStateConsistencyComputer::~DefaultLoadedStateConsistencyComputer()
{
}

void DefaultLoadedStateConsistencyComputer::Reset()
{
	hasValidCarrier_ = hasValidLoadable_ = false;
}
void DefaultLoadedStateConsistencyComputer::EnableCarrier(bool value)
{
	hasValidCarrier_ |= value;
}
void DefaultLoadedStateConsistencyComputer::EnableLoadable(bool value)
{
	hasValidLoadable_ |= value;
}
bool DefaultLoadedStateConsistencyComputer::HasValidCarrier() const
{
	return hasValidCarrier_;
}
bool DefaultLoadedStateConsistencyComputer::HasValidLoadable() const
{
	return hasValidLoadable_;
}
}
