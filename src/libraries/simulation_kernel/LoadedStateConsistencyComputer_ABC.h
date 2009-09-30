/*
 * LoadedStateConsistencyComputer_ABC.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_LoadedStateConsistencyComputer_ABC_H__
#define __transport_LoadedStateConsistencyComputer_ABC_H__

namespace transport
{

class LoadedStateConsistencyComputer_ABC
{
public:
	LoadedStateConsistencyComputer_ABC();
	virtual ~LoadedStateConsistencyComputer_ABC();

	virtual void Reset() =0;
	virtual void EnableCarrier(bool value) =0;
	virtual void EnableLoadable(bool value) =0;
	virtual bool HasValidCarrier() const =0;
	virtual bool HasValidLoadable() const =0;
};

}

#endif /* __transport_LoadedStateConsistencyComputer_ABC_H__ */
