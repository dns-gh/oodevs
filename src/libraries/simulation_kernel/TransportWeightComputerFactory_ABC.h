/*
 * TransportWeightComputerFactory_ABC.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportWeightComputerFactory_ABC_H__
#define __transport_TransportWeightComputerFactory_ABC_H__

namespace transport
{
class TransportWeightComputer_ABC;
class TransportStrategy_ABC;

class TransportWeightComputerFactory_ABC
{
public:
	TransportWeightComputerFactory_ABC();
	virtual ~TransportWeightComputerFactory_ABC();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	virtual TransportWeightComputer_ABC& Create(const TransportStrategy_ABC*) const =0;
};

}

#endif /* __transport_TransportWeightComputerFactory_ABC_H__ */
