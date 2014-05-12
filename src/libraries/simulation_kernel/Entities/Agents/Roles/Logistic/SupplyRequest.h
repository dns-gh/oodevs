// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyRequest_h_
#define __SupplyRequest_h_

#include "SupplyRequest_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace logistic
{
    class SupplySupplier_ABC;

// =============================================================================
/** @class  SupplyRequest
    @brief  SupplyRequest
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplyRequest : public SupplyRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyRequest( const PHY_DotationCategory& dotationCategory, unsigned int recipientId );
             SupplyRequest();
    virtual ~SupplyRequest();
    //@}

    //! @name Operations
    //@{
    virtual void AddResource( const boost::shared_ptr< SupplyResource_ABC >& resource, const MIL_AgentPion& pion, double quantity );
    virtual bool AffectSupplier( SupplySupplier_ABC& supplier );
    virtual bool AffectSupplier( SupplyRecipient_ABC& recipient, const boost::shared_ptr< LogisticLink_ABC >& supplier );

    virtual bool HasRequester( const MIL_AgentPion& pion ) const;
    virtual bool HasRequesterDestroyed() const;

    virtual double Supply          ();
    virtual double Convoy          ( double quantity ); // Return the quantity convoyed
    virtual double RemoveFromConvoy( double quantity ); // Return the quantity removed

    virtual void ReserveStock();
    virtual void ReturnStockNotAssignedToConvoy();

    virtual void NotifySuperiorNotAvailable( SupplyRecipient_ABC& recipient );
    //@}

    //! @name Accessors
    //@{
    virtual       SupplySupplier_ABC*   GetSupplier         () const;
    virtual       unsigned int          GetId               () const;
    virtual const PHY_DotationCategory& GetDotationCategory () const;
    virtual       double                GetGrantedQuantity  () const;
    virtual       double                GetRequestedQuantity() const;
    virtual       bool                  IsComplementary     () const;
    virtual const MIL_Agent_ABC*        GetProvider         () const;
    //@}

    //! @name Network
    //@{
    virtual void SendCreation   () const;
    virtual void SendDestruction() const;
    virtual void SendChangedState() const;
    virtual void SendFullState   () const;
    virtual void Clean           ();

    virtual void Serialize( sword::SupplyResourceRequest& msg ) const;
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< boost::shared_ptr< SupplyResource_ABC > , double /*quantity requested*/ > > T_Resources;
    typedef std::vector< const MIL_AgentPion* > T_Requesters;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateRequestedQuantities( double authorizedQuantity );
    void SendState() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    unsigned int recipientId_;
    const PHY_DotationCategory* dotationCategory_;
    sword::LogSupplyRequestUpdate_EnumLogSupplyRequestStatus state_;
    T_Resources resources_;
    T_Requesters requesters_;
    SupplySupplier_ABC* supplier_;
    double requestedQuantity_;
    double grantedQuantity_;
    double convoyedQuantity_;
    double suppliedQuantity_;
    bool complementarySupply_;
    boost::shared_ptr< LogisticLink_ABC > supplierQuotas_;
    const MIL_Agent_ABC* provider_;
    bool needNetworkUpdate_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( logistic::SupplyRequest )

#endif // __SupplyRequest_ABC_h_
