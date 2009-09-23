/*
 * DefaultTransportCapacityComputer.h
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_DefaultTransportCapacityComputer_H__
#define __transport_DefaultTransportCapacityComputer_H__

#include "TransportCapacityComputer_ABC.h"

namespace transport
{

class DefaultTransportCapacityComputer : public TransportCapacityComputer_ABC
{
public:
	DefaultTransportCapacityComputer();
	virtual ~DefaultTransportCapacityComputer();

public:
	//! @name Operations
	//@{
	virtual void Reset();
	virtual void ApplyOnComposante( PHY_ComposantePion& comp );
	virtual double WeightCapacity() const;
	virtual double WeightLoadedPerTimeStep() const;
	virtual double WeightUnloadedPerTimeStep() const;
	virtual double MaxComposanteTransportedWeight() const;
	//@}

private:
	double rWeightCapacity_;
	double rWeightLoadedPerTimeStep_;
	double rWeightUnloadedPerTimeStep_;
    double rMaxComposanteTransportedWeight_;
};

}

#endif /* __transport_DefaultTransportCapacityComputer_H__ */
