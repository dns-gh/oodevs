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
class TransportWeightComputer_ABC;
class TransportPermissionComputer_ABC;
class TransportStrategy_ABC;

class TransportComputerFactory_ABC
{
public:
	TransportComputerFactory_ABC();
	virtual ~TransportComputerFactory_ABC();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	//! @name Operations
	//@{
	virtual TransportCapacityComputer_ABC& CreateCapacityComputer( ) const = 0;
	virtual TransportWeightComputer_ABC& CreateWeightComputer(const TransportStrategy_ABC*) const =0;
	virtual TransportPermissionComputer_ABC& CreatePermissionComputer() const = 0;
	//@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_ABC_H__ */
