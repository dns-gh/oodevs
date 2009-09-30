/*
 * LoadingComputerFactory.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_LoadingComputerFactory_H__
#define __transport_LoadingComputerFactory_H__

#include "LoadingComputerFactory_ABC.h"

namespace transport
{

class LoadingComputerFactory: public transport::LoadingComputerFactory_ABC
{
public:
	LoadingComputerFactory();
	virtual ~LoadingComputerFactory();

	HumanLoadingTimeComputer_ABC& CreateHumanLoadingTimeComputer();
	LoadedStateConsistencyComputer_ABC& CreateLoadedStateConsistencyComputer();
private:
	std::auto_ptr<HumanLoadingTimeComputer_ABC> humanLoadingTimeComputer_;
	std::auto_ptr<LoadedStateConsistencyComputer_ABC> loadedStateConsistencyComputer_;
};

}

#endif /* __transport_LoadingComputerFactory_H__ */
