/*
 * DefaultConsumptionModeChangeRequest.cpp
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "DefaultConsumptionModeChangeRequest.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"

namespace dotation
{

DefaultConsumptionModeChangeRequest::DefaultConsumptionModeChangeRequest() :
	newMode_(0),
	failed_(false)
{
}

DefaultConsumptionModeChangeRequest::~DefaultConsumptionModeChangeRequest()
{
}

void DefaultConsumptionModeChangeRequest::Reset(const PHY_ConsumptionType& newMode)
{
	newMode_ = &newMode;
	failed_=false;
	changedDotations_.clear();
}
const PHY_ConsumptionType& DefaultConsumptionModeChangeRequest::GetType() const
{
	return *newMode_;
}
void DefaultConsumptionModeChangeRequest::ConsumptionModeChanged(bool changed, PHY_RoleInterface_Dotations* const dotation)
{
	if(!failed_)
	{
		if(!changed)
		{
			Rollback();
			failed_ = true;
		}
		else
		{
			changedDotations_.push_back(dotation);
		}
	}
}
bool DefaultConsumptionModeChangeRequest::AllChanged() const
{
	return !failed_;
}

void DefaultConsumptionModeChangeRequest::Rollback()
{
	std::for_each(changedDotations_.begin(),changedDotations_.end(), std::mem_fun(&PHY_RoleInterface_Dotations::RollbackConsumptionMode));
}
}
