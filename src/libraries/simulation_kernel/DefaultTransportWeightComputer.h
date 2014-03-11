// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_DefaultTransportWeightComputer_h__
#define __transport_DefaultTransportWeightComputer_h__

#include "TransportWeightComputer_ABC.h"

namespace transport
{

// =============================================================================
/** @class  DefaultTransportWeightComputer
    @brief  DefaultTransportWeightComputer
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultTransportWeightComputer : public TransportWeightComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultTransportWeightComputer( const TransportStrategy_ABC* );
    virtual ~DefaultTransportWeightComputer();
    //@}

    //! @name Operations
    //@{
    virtual void AddTransportedWeight( double weight, bool canBeLoaded );
    virtual double TotalTransportedWeight() const;
    virtual double HeaviestTransportedWeight() const;
    //@}

private:
    //! @name Attributes
    //@{
    double totalTransportedWeight_;
    double heaviestTransportedWeight_;
    const TransportStrategy_ABC* strategy_;
    //@}
};

}

#endif /* __transport_DefaultTransportWeightComputer_h__ */
