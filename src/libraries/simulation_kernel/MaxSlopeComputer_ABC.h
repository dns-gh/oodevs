/*
 * MaxSlopeComputer_ABC.h
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#ifndef __moving_MaxSlopeComputer_ABC_H__
#define __moving_MaxSlopeComputer_ABC_H__

#include "simulation_kernel/OnComponentComputer_ABC.h"

namespace moving
{

class MaxSlopeComputer_ABC : public OnComponentComputer_ABC
{
public:
	MaxSlopeComputer_ABC();
	virtual ~MaxSlopeComputer_ABC();

	virtual void Reset() =0;
	virtual double GetMaxSlope() const =0;
};

}

#endif /* __moving_MaxSlopeComputer_ABC_H__ */
