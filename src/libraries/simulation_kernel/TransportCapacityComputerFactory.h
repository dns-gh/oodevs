/*
 * TransportCapacityComputerFactory.h
 *
 *  Created on: 23 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportCapacityComputerFactory_H__
#define __transport_TransportCapacityComputerFactory_H__

#include "TransportCapacityComputerFactory_ABC.h"

namespace transport {

class TransportCapacityComputerFactory : public TransportCapacityComputerFactory_ABC
{
public:
	TransportCapacityComputerFactory();
	virtual ~TransportCapacityComputerFactory();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	//! @name Operations
	//@{
	virtual TransportCapacityComputer_ABC& Create( ) const ;
	//@}
private:
	//! @name Attribute
	//@{
	std::auto_ptr< TransportCapacityComputer_ABC > pTransportCapacityComputer_;
	//@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_H__ */
