/*
 * LoadingComputerFactory_ABC.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_LoadingComputerFactory_ABC_H__
#define __transport_LoadingComputerFactory_ABC_H__

namespace transport
{
class HumanLoadingTimeComputer_ABC;
class LoadedStateConsistencyComputer_ABC;

class LoadingComputerFactory_ABC
{
public:
	LoadingComputerFactory_ABC();
	virtual ~LoadingComputerFactory_ABC();

	virtual HumanLoadingTimeComputer_ABC& CreateHumanLoadingTimeComputer() =0;
	virtual LoadedStateConsistencyComputer_ABC& CreateLoadedStateConsistencyComputer() =0;
};

}

#endif /* __transport_LoadingComputerFactory_ABC_H__ */
