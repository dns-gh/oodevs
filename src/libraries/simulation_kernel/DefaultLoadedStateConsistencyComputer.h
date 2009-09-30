/*
 * DefaultLoadedStateConsistencyComputer.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_DefaultLoadedStateConsistencyComputer_H__
#define __transport_DefaultLoadedStateConsistencyComputer_H__

#include "LoadedStateConsistencyComputer_ABC.h"

namespace transport
{

class DefaultLoadedStateConsistencyComputer: public transport::LoadedStateConsistencyComputer_ABC
{
public:
	DefaultLoadedStateConsistencyComputer();
	virtual ~DefaultLoadedStateConsistencyComputer();

	void Reset();
	void EnableCarrier(bool value);
	void EnableLoadable(bool value);
	bool HasValidCarrier() const;
	bool HasValidLoadable() const;
private:
	bool hasValidCarrier_;
	bool hasValidLoadable_;
};

}

#endif /* __transport_DefaultLoadedStateConsistencyComputer_H__ */
