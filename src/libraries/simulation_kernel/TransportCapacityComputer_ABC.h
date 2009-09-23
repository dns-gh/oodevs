/*
 * TransportCapacityComputer_ABC.h
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportCapacityComputer_ABC_H__
#define __transport_TransportCapacityComputer_ABC_H__


class PHY_ComposantePion;


namespace transport {

class TransportCapacityComputer_ABC {
public:
	virtual ~TransportCapacityComputer_ABC();
	TransportCapacityComputer_ABC();

public:
	//! @name Operations
	//@{
	virtual void Reset() = 0;
	virtual void ApplyOnComposante( PHY_ComposantePion& comp ) = 0;
	virtual double WeightCapacity() const = 0;
	virtual double WeightLoadedPerTimeStep() const = 0;
	virtual double WeightUnloadedPerTimeStep() const = 0;
	virtual double MaxComposanteTransportedWeight() const = 0;
	//@}
};

}

#endif /* __transport_TransportCapacityComputer_ABC_H__ */
