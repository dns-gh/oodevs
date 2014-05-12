// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplySupplier_ABC_h_
#define __SupplySupplier_ABC_h_

class PHY_DotationCategory;
class PHY_ComposantePion;
class PHY_ComposanteTypePion;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_AutomateLOG;

namespace sword
{
    class ParentEntity;
}

namespace logistic
{
    class SupplyConsign_ABC;
    class SupplyConvoyReal_ABC;

// =============================================================================
/** @class  SupplySupplier_ABC
    @brief  SupplySupplier_ABC
*/
// Created: NLD 2011-01-10
// =============================================================================
class SupplySupplier_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplySupplier_ABC() {}
    virtual ~SupplySupplier_ABC() {}
    //@}

    struct Stock
    {
        double quantity_;
        const MIL_Agent_ABC* provider_;
    };
    //! @name Operations
    //@{
    virtual const MT_Vector2D&   GetPosition          () const = 0;
    virtual const MIL_AgentPion* GetPC                () const = 0; //$$$ Bullshit
    virtual       unsigned int   GetSupplierId        () const = 0;
    virtual       bool           BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const = 0;

    virtual bool   SupplyHasStock                     ( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual Stock  SupplyGetStock                     ( const PHY_DotationCategory& dotationCategory, double quantity ) const = 0;
    virtual bool   SupplyReturnStock                  ( const PHY_DotationCategory& dotationCategory, double quantity ) const = 0;
    virtual void   SupplyHandleRequest                ( const boost::shared_ptr < logistic::SupplyConsign_ABC >& consign ) = 0;
    virtual bool   SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual bool   SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_ComposanteTypePion& transporterType ) const = 0;

    virtual MIL_AgentPion* SupplyCreateConvoyPion     ( const MIL_AgentTypePion& type, const boost::shared_ptr< logistic::SupplyConvoyReal_ABC >& convoy ) = 0;
    virtual void           SupplyDestroyConvoyPion    ( MIL_AgentPion& convoyPion ) = 0;
    //@}

    //! @name Events
    //@{
    virtual void OnSupplyConvoyArriving( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    virtual void OnSupplyConvoyLeaving ( const boost::shared_ptr< SupplyConsign_ABC >& consign ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::ParentEntity& message ) const = 0;
    template < typename Archive > void serialize( Archive&, const unsigned int )
    {}
    //@}
};

}

#endif // __SupplySupplier_ABC_h_
