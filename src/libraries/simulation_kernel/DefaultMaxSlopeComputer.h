/*
 * DefaultMaxSlopeComputer.h
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#ifndef __moving_DefaultMaxSlopeComputer_H__
#define __moving_DefaultMaxSlopeComputer_H__

#include "simulation_kernel/MaxSlopeComputer_ABC.h"

namespace moving
{

class DefaultMaxSlopeComputer : public MaxSlopeComputer_ABC
{
public:
	DefaultMaxSlopeComputer();
	virtual ~DefaultMaxSlopeComputer();

	void ApplyOnComponent( PHY_ComposantePion& component );
	void Reset();
	double GetMaxSlope() const;
private:
	double maxSlope_;
	bool hasUsableComponent_;
};

}

#endif /* __moving_DefaultMaxSlopeComputer_H__ */
