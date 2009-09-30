/*
 * DefaultHumanLoadingTimeComputer.h
 *
 *  Created on: 30 sept. 2009
 *      Author: ahc
 */

#ifndef DEFAULTHUMANLOADINGTIMECOMPUTER_H_
#define DEFAULTHUMANLOADINGTIMECOMPUTER_H_

#include "HumanLoadingTimeComputer_ABC.h"

namespace transport
{

class DefaultHumanLoadingTimeComputer: public transport::HumanLoadingTimeComputer_ABC
{
public:
	DefaultHumanLoadingTimeComputer();
	virtual ~DefaultHumanLoadingTimeComputer();

	void Reset();
	void AddHumans(int humansCount);
	void AddTime(double loadingPerTimeStep, double unloadingPerTimeStep);
	int GetHumansCount() const;
	double GetHumansLoadedPerTimeStep() const;
	double GetHumansUnloadedPerTimeStep() const;
private:
	int humansCount_;
	double humansLoadedPerTimeStep_;
	double humansUnloadedPerTimeStep_;
};

}

#endif /* DEFAULTHUMANLOADINGTIMECOMPUTER_H_ */
