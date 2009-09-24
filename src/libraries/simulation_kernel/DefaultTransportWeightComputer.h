/*
 * DefaultTransportWeightComputer.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_DefaultTransportWeightComputer_h__
#define __transport_DefaultTransportWeightComputer_h__

#include "TransportWeightComputer_ABC.h"

namespace transport
{

class DefaultTransportWeightComputer : public TransportWeightComputer_ABC
{
public:
    DefaultTransportWeightComputer();
    virtual ~DefaultTransportWeightComputer();

    virtual void Reset(const TransportStrategy_ABC*);
    virtual void AddTransportedWeight(double weight, bool canBeLoaded) ;
    virtual double TotalTransportedWeight() const ;
    virtual double HeaviestTransportedWeight() const ;

private:
    double totalTransportedWeight_;
    double heaviestTransportedWeight_;
    const TransportStrategy_ABC* strategy_;
};

}

#endif /* __transport_DefaultTransportWeightComputer_h__ */
