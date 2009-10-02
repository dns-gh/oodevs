/*
 * DefaultMaxSlopeComputer.cpp
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#include "simulation_kernel_pch.h"

#include "DefaultMaxSlopeComputer.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace moving
{

DefaultMaxSlopeComputer::DefaultMaxSlopeComputer() :
		maxSlope_ (std::numeric_limits<double>::max()),
		hasUsableComponent_(false)
{
}

DefaultMaxSlopeComputer::~DefaultMaxSlopeComputer()
{
}

void DefaultMaxSlopeComputer::ApplyOnComponent( PHY_ComposantePion& component )
{
	 if( component.CanMove() )
	{
		 hasUsableComponent_ = true;
		maxSlope_ = std::min( maxSlope_, component.GetType().GetMaxSlope() );
	}
}
void DefaultMaxSlopeComputer::Reset()
{
	maxSlope_ = std::numeric_limits<double>::max();
	hasUsableComponent_ = false;
}
double DefaultMaxSlopeComputer::GetMaxSlope() const
{
	return hasUsableComponent_ ? maxSlope_ : 0;
}

}
