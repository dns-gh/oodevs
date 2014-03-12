// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportWeightComputer_ABC_H__
#define __transport_TransportWeightComputer_ABC_H__

namespace transport
{
// =============================================================================
/** @class  TransportWeightComputer_ABC
    @brief  TransportWeightComputer_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportWeightComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportWeightComputer_ABC() {}
    virtual ~TransportWeightComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AddTransportedWeight( double weight, bool canBeLoaded ) = 0;
    virtual bool CanTransportDestroyed() const = 0;
    //@}
};

}

#endif /* __transport_TransportWeightComputer_ABC_H__ */
