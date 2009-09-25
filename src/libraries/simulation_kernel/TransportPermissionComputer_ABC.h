/*
 * TransportPermissionComputer_ABC.h
 *
 *  Created on: 25 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportPermissionComputer_ABC_H__
#define __transport_TransportPermissionComputer_ABC_H__

namespace transport
{

class TransportPermissionComputer_ABC
{
public:
	TransportPermissionComputer_ABC();
	virtual ~TransportPermissionComputer_ABC();

	virtual void Reset() =0;
	virtual void AllowLoading(bool doAllow) =0;
	virtual bool CanBeLoaded() const = 0;
};

}

#endif /* __transport_TransportPermissionComputer_ABC_H__ */
