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

class TransportComputerFactory : public TransportComputerFactory_ABC
{
public:
	TransportComputerFactory();
	virtual ~TransportComputerFactory();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	//! @name Operations
	//@{
	TransportCapacityComputer_ABC& CreateCapacityComputer( ) const ;
	TransportWeightComputer_ABC& CreateWeightComputer(const TransportStrategy_ABC*) const;
	TransportPermissionComputer_ABC& CreatePermissionComputer() const;
	//@}
private:
	//! @name Attribute
	//@{
	std::auto_ptr< TransportCapacityComputer_ABC > capacityComputer_;
	std::auto_ptr<TransportWeightComputer_ABC> weightComputer_;
	std::auto_ptr<TransportPermissionComputer_ABC> permissionComputer_;
	//@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_H__ */
