// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_DefaultTransportCapacityComputer_H__
#define __transport_DefaultTransportCapacityComputer_H__

#include "TransportCapacityComputer_ABC.h"

namespace transport
{

// =============================================================================
/** @class  DefaultTransportCapacityComputer
    @brief  DefaultTransportCapacityComputer
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultTransportCapacityComputer : public TransportCapacityComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    DefaultTransportCapacityComputer();
    virtual ~DefaultTransportCapacityComputer();
    //@}

public:
    //! @name Operations
    //@{
    virtual void ApplyOnComposante( PHY_ComposantePion& comp );
    virtual double WeightCapacity() const;
    virtual double WeightLoadedPerTimeStep() const;
    virtual double WeightUnloadedPerTimeStep() const;
    virtual double MaxComposanteTransportedWeight() const;
    //@}

private:
    //! @name Attributes
    //@{
    double rWeightCapacity_;
    double rWeightLoadedPerTimeStep_;
    double rWeightUnloadedPerTimeStep_;
    double rMaxComposanteTransportedWeight_;
    //@}
};

}

#endif /* __transport_DefaultTransportCapacityComputer_H__ */
