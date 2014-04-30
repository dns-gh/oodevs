// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequest_ABC_h_
#define __SupplyRequest_ABC_h_

#include <boost/noncopyable.hpp>

class PHY_DotationCategory;
class MIL_AgentPion;
class MIL_Agent_ABC;

namespace sword
{
    class SupplyResourceRequest;
}

namespace logistic
{
    class SupplyRecipient_ABC;
    class SupplyResource_ABC;
    class SupplySupplier_ABC;
    class LogisticLink_ABC;

// =============================================================================
/** @class  SupplyRequest_ABC
    @brief  SupplyRequest_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequest_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequest_ABC() {}
    virtual ~SupplyRequest_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AddResource( const boost::shared_ptr< SupplyResource_ABC >& resource, const MIL_AgentPion& pion, double quantity ) = 0;
    virtual bool AffectSupplier( SupplySupplier_ABC& supplier ) = 0;
    virtual bool AffectSupplier( SupplyRecipient_ABC& recipient, const boost::shared_ptr< LogisticLink_ABC >& supplier ) = 0;

    virtual double Supply          () = 0; // Return the quantity supplied;
    virtual double Convoy          ( double quantity ) = 0; // Return the quantity convoyed
    virtual double RemoveFromConvoy( double quantity ) = 0; // Return the quantity removed

    virtual void ReserveStock() = 0;
    virtual void ReturnStockNotAssignedToConvoy() = 0;

    virtual bool HasRequester( const MIL_AgentPion& pion ) const = 0;
    virtual bool HasRequesterDestroyed() const = 0;

    virtual bool IsComplementary() const = 0;

    virtual void NotifySuperiorNotAvailable( SupplyRecipient_ABC& recipient ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual       SupplySupplier_ABC*   GetSupplier         () const = 0;
    virtual       unsigned int          GetId               () const = 0;
    virtual const PHY_DotationCategory& GetDotationCategory () const = 0;
    virtual       double                GetGrantedQuantity  () const = 0;
    virtual       double                GetRequestedQuantity() const = 0;
    virtual const MIL_Agent_ABC*        GetProvider         () const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendCreation    () const = 0;
    virtual void SendDestruction () const = 0;
    virtual void SendChangedState() const = 0;
    virtual void SendFullState   () const = 0;
    virtual void Clean           () = 0;

    virtual void Serialize( sword::SupplyResourceRequest& msg ) const = 0;
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

}

#endif // __SupplyRequest_ABC_h_
