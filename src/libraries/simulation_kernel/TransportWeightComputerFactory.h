/*
 * TransportWeightComputerFactory.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportWeightComputerFactory_H__
#define __transport_TransportWeightComputerFactory_H__

#include "TransportWeightComputerFactory_ABC.h"

namespace transport
{

class TransportWeightComputerFactory : public TransportWeightComputerFactory_ABC
{
public:
	TransportWeightComputerFactory();
	virtual ~TransportWeightComputerFactory();

	template< typename Archive > void serialize( Archive&, const uint ) {}

	virtual TransportWeightComputer_ABC& Create(const TransportStrategy_ABC*) const;
private:
	std::auto_ptr<TransportWeightComputer_ABC> computer_;
};

}

#endif /* __transport_TransportWeightComputerFactory_H__ */
