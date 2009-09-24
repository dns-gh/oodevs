/*
 * TransportWeightComputerFactory.cpp
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "TransportWeightComputerFactory.h"
#include "DefaultTransportWeightComputer.h"

namespace transport
{

TransportWeightComputerFactory::TransportWeightComputerFactory() :
	computer_(new DefaultTransportWeightComputer())
{
}

TransportWeightComputerFactory::~TransportWeightComputerFactory()
{
}

TransportWeightComputer_ABC& TransportWeightComputerFactory::Create(const TransportStrategy_ABC* st) const
{
	computer_->Reset(st);
	return *computer_;
}

}
