/*
 * TransportCapacityComputerFactory.cpp
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "TransportCapacityComputerFactory.h"
#include "DefaultTransportCapacityComputer.h"
#include "DefaultTransportWeightComputer.h"
#include "DefaultTransportPermissionComputer.h"

namespace transport {

TransportComputerFactory::TransportComputerFactory() :
	capacityComputer_(new DefaultTransportCapacityComputer()),
	weightComputer_(new DefaultTransportWeightComputer()),
	permissionComputer_(new DefaultTransportPermissionComputer())
{

}

TransportComputerFactory::~TransportComputerFactory()
{
}

TransportCapacityComputer_ABC& TransportComputerFactory::CreateCapacityComputer( ) const
{
	capacityComputer_->Reset();
	return *capacityComputer_;
}

TransportWeightComputer_ABC& TransportComputerFactory::CreateWeightComputer(const TransportStrategy_ABC* st) const
{
	weightComputer_->Reset(st);
	return *weightComputer_;
}

TransportPermissionComputer_ABC& TransportComputerFactory::CreatePermissionComputer() const
{
	permissionComputer_->Reset();
	return *permissionComputer_;
}

}
