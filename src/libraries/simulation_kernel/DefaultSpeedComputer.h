/*
 * DefaultSpeedComputer.h
 *
 *  Created on: 1 oct. 2009
 *      Author: ahc
 */

#ifndef DEFAULTSPEEDCOMPUTER_H_
#define DEFAULTSPEEDCOMPUTER_H_

#include "SpeedComputer_ABC.h"

namespace moving
{

class DefaultSpeedComputer : public SpeedComputer_ABC
{
public:
	DefaultSpeedComputer();
	virtual ~DefaultSpeedComputer();

	void Reset(const SpeedStrategy_ABC* strategy);
	void ApplyOnComponent(const PHY_ComposantePion&);
	void ApplyOnReinforcement( MIL_AgentPion&);
	void ApplyOnPopulation( const DEC_Knowledge_PopulationCollision&);
	void AddModifier(double ratio, bool isMax=true);
	double GetSpeed() const;
private:
	const SpeedStrategy_ABC* strategy_;
	double speed_;
	bool hasUsableComponent_;
	double speedRatio_;
};

}

#endif /* DEFAULTSPEEDCOMPUTER_H_ */
