/*
 * ConsumptionChangeRequestHandler_ABC.h
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#ifndef __dotation_ConsumptionChangeRequestHandler_ABC_H__
#define __dotation_ConsumptionChangeRequestHandler_ABC_H__

namespace dotation
{

class ConsumptionModeChangeRequest_ABC;

class ConsumptionChangeRequestHandler_ABC
{
public:
	ConsumptionChangeRequestHandler_ABC();
	virtual ~ConsumptionChangeRequestHandler_ABC();

	virtual void ChangeConsumptionMode(ConsumptionModeChangeRequest_ABC& request)=0;
};

}

#endif /* __dotation_ConsumptionChangeRequestHandler_ABC_H__ */
