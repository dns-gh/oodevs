/*
 * TansportWeightComputer_ABC.h
 *
 *  Created on: 24 sept. 2009
 *      Author: ahc
 */

#ifndef __transport_TransportWeightComputer_ABC_H__
#define __transport_TransportWeightComputer_ABC_H__

namespace transport
{

class TransportStrategy_ABC
{
public:
	virtual bool Autorize (bool canBeLoaded) const =0;
};

class TransportWeightComputer_ABC
{

public:
	TransportWeightComputer_ABC();
    virtual ~TransportWeightComputer_ABC();

    virtual void Reset(const TransportStrategy_ABC* ) =0;
    virtual void AddTransportedWeight(double weight, bool canBeLoaded=false) =0;
    virtual double TotalTransportedWeight() const =0;
    virtual double HeaviestTransportedWeight() const =0;
};

}

#endif /* __transport_TransportWeightComputer_ABC_H__ */
