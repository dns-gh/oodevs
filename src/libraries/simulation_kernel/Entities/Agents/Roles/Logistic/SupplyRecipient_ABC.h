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
#include <boost/noncopyable.hpp>

class MIL_AgentPion;
class MIL_AutomateLOG;
class PHY_DotationCategory;

namespace sword
{
    class Id;
}

namespace logistic
{
    class SupplyConsign_ABC;

// =============================================================================
/** @class  SupplyRecipient_ABC
    @brief  SupplyRecipient_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRecipient_ABC : boost::noncopyable
{
protected:
    //! @name Types
    //@{
    typedef std::vector< const MIL_AgentPion* > T_Requesters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             SupplyRecipient_ABC() {}
    virtual ~SupplyRecipient_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const MT_Vector2D&   GetPosition          () const = 0;
    virtual const MIL_AgentPion* GetPC                () const = 0; //$$$ Bullshit
    virtual       unsigned int   GetRecipientId       () const = 0;
    virtual       bool           BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const = 0;
    //@}

    //! @name Events
    //@{
    virtual void OnSupplyCanceled      ( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyDone          ( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyScheduled     ( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;

    virtual void NotifySuperiorNotAvailable( const T_Requesters& requesters ) = 0;
    virtual void NotifyStockNotAvailable( const PHY_DotationCategory& dotationCategory, const T_Requesters& requesters ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::Id& message ) const = 0;

    template < typename Archive >
    void serialize( Archive&, const unsigned int )
    {}
    //@}
};

}

#endif // __SupplyConveyor_ABC_h_
