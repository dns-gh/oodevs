/*
 * TransportPermissionComputer.h
 *
 *  Created on: 25 sept. 2009
 *      Author: ahc
 */

#ifndef __transportTransportPermissionComputer_H__
#define __transportTransportPermissionComputer_H__

#include "TransportPermissionComputer_ABC.h"

namespace transport
{

class DefaultTransportPermissionComputer: public transport::TransportPermissionComputer_ABC
{
public:
	DefaultTransportPermissionComputer();
	virtual ~DefaultTransportPermissionComputer();

	void Reset();
	void AllowLoading(bool doAllow);
	bool CanBeLoaded() const;

private:
	bool  allow_;
};

}

#endif /* __transportTransportPermissionComputer_H__ */
