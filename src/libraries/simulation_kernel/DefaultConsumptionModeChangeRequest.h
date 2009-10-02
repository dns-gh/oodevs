/*
 * DefaultConsumptionModeChangeRequest.h
 *
 *  Created on: 2 oct. 2009
 *      Author: ahc
 */

#ifndef __dotation_DefaultConsumptionModeChangeRequest_H__
#define __dotation_DefaultConsumptionModeChangeRequest_H__

#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"
#include <vector>

namespace dotation
{

class DefaultConsumptionModeChangeRequest : public ConsumptionModeChangeRequest_ABC
{
public:
	DefaultConsumptionModeChangeRequest();
	virtual ~DefaultConsumptionModeChangeRequest();

	virtual void Reset(const PHY_ConsumptionType& newMode) ;
	virtual const PHY_ConsumptionType& GetType() const;
	virtual void ConsumptionModeChanged(bool changed, PHY_RoleInterface_Dotations* const dotation);
	virtual bool AllChanged() const;

private:
	void Rollback();

private:
	const PHY_ConsumptionType* newMode_;
	std::vector<PHY_RoleInterface_Dotations* const> changedDotations_;
	bool failed_;
};

}

#endif /* __dotation_DefaultConsumptionModeChangeRequest_H__ */
