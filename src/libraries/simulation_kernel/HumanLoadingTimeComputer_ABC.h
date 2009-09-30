/*
 * HumanLoadingTimeComputer_ABC.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_HumanLoadingTimeComputer_ABC_H__
#define __transport_HumanLoadingTimeComputer_ABC_H__

namespace transport
{

class HumanLoadingTimeComputer_ABC
{
public:
	HumanLoadingTimeComputer_ABC();
	virtual ~HumanLoadingTimeComputer_ABC();

	virtual void Reset() =0;
	virtual void AddHumans(int humansCount) =0;
	virtual void AddTime(double loadingPerTimeStep, double unloadingPerTimeStep) =0;
	virtual int GetHumansCount() const =0;
	virtual double GetHumansLoadedPerTimeStep() const =0;
	virtual double GetHumansUnloadedPerTimeStep() const =0;
};

}

#endif /* __transport_HumanLoadingTimeComputer_ABC_H__ */
