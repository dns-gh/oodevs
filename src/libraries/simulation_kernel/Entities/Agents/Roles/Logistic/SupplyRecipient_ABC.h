// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRecipient_ABC_h_
#define __SupplyRecipient_ABC_h_

#include "MT_Tools/MT_Vector2DTypes.h"

class MIL_AgentPion;
class MIL_AutomateLOG;

namespace sword {
    class AutomatId;
}

namespace logistic {
    class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyRecipient_ABC
    @brief  SupplyRecipient_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRecipient_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRecipient_ABC() {}
    virtual ~SupplyRecipient_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MT_Vector2D&   GetPosition          () const = 0;
    virtual const MIL_AgentPion& GetPC                () const = 0; //$$$ Bullshit
    virtual       bool           BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const = 0;
    //@}

    //! @name Events
    //@{
    virtual void OnSupplyCanceled      () = 0;
    virtual void OnSupplyDone          () = 0;
    virtual void OnSupplyConvoyArriving( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign ) = 0;
    virtual void OnSupplyConvoyLeaving ( boost::shared_ptr< const SupplyConsign_ABC > supplyConsign ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::AutomatId& message ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplyConveyor_ABC_h_
