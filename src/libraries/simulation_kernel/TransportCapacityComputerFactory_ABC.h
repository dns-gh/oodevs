/*
 * TransportCapacityComputerFactory_ABC.h
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportCapacityComputerFactory_ABC_H__
#define __transport_TransportCapacityComputerFactory_ABC_H__

namespace transport {

class TransportCapacityComputer_ABC;

class TransportCapacityComputerFactory_ABC
{
public:
	TransportCapacityComputerFactory_ABC();
	virtual ~TransportCapacityComputerFactory_ABC();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	//! @name Operations
	//@{
	virtual TransportCapacityComputer_ABC& Create( ) const = 0;
	//@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_ABC_H__ */
