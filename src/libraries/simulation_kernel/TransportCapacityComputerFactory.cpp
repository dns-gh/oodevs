/*
 * TransportCapacityComputerFactory.cpp
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "TransportCapacityComputerFactory.h"
#include "DefaultTransportCapacityComputer.h"

namespace transport {

TransportCapacityComputerFactory::TransportCapacityComputerFactory()
	: pTransportCapacityComputer_(new DefaultTransportCapacityComputer())
{

}

TransportCapacityComputerFactory::~TransportCapacityComputerFactory()
{
}

TransportCapacityComputer_ABC& TransportCapacityComputerFactory::Create( ) const
{
	pTransportCapacityComputer_->Reset();
	return *pTransportCapacityComputer_;
}

}
