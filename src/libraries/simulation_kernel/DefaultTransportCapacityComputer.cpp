/*
 * DefaultTransportCapacityComputer.cpp
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "DefaultTransportCapacityComputer.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace transport {

DefaultTransportCapacityComputer::DefaultTransportCapacityComputer() :
	rWeightCapacity_                ( 0. )
	, rWeightLoadedPerTimeStep_       ( 0. )
	, rWeightUnloadedPerTimeStep_     ( 0. )
	, rMaxComposanteTransportedWeight_( 0. )
{
	// TODO Auto-generated constructor stub

}

DefaultTransportCapacityComputer::~DefaultTransportCapacityComputer()
{
}

void DefaultTransportCapacityComputer::Reset()
{
	rWeightCapacity_=
		rWeightLoadedPerTimeStep_       =
		rWeightUnloadedPerTimeStep_     =
		rMaxComposanteTransportedWeight_ = 0. ;
}

void DefaultTransportCapacityComputer::ApplyOnComposante( PHY_ComposantePion& composante )
{
	if( composante.CanTransportPion() )
	{
		rWeightCapacity_                 += composante.GetPionTransporterWeightCapacity           ();
		rWeightLoadedPerTimeStep_        += composante.GetPionTransporterWeightLoadedPerTimeStep  ();
		rWeightUnloadedPerTimeStep_      += composante.GetPionTransporterWeightUnloadedPerTimeStep();
		rMaxComposanteTransportedWeight_  = std::max( rMaxComposanteTransportedWeight_, composante.GetPionTransporterWeightCapacity() );
	}
}
double DefaultTransportCapacityComputer::WeightCapacity() const
{
	return rWeightCapacity_;
}
double DefaultTransportCapacityComputer::WeightLoadedPerTimeStep() const
{
	return rWeightLoadedPerTimeStep_;
}
double DefaultTransportCapacityComputer::WeightUnloadedPerTimeStep() const
{
	return rWeightUnloadedPerTimeStep_;
}
double DefaultTransportCapacityComputer::MaxComposanteTransportedWeight() const
{
	return rMaxComposanteTransportedWeight_;
}
}
