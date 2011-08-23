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
class MIL_AgentPion;
class MIL_AgentTypePion;

namespace sword {
    class ParentEntity;
}

namespace logistic {
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

    //! @name Operations
    //@{
    virtual const MT_Vector2D&   GetPosition() const = 0;
    virtual const MIL_AgentPion* GetPC      () const = 0; //$$$ Bullshit

    virtual bool   SupplyHasStock                     ( const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual double SupplyGetStock                     ( const PHY_DotationCategory& dotationCategory, double quantity ) const = 0;
    virtual bool   SupplyReturnStock                  ( const PHY_DotationCategory& dotationCategory, double quantity ) const = 0;
    virtual void   SupplyHandleRequest                ( boost::shared_ptr < logistic::SupplyConsign_ABC > consign ) = 0;
    virtual bool   SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_DotationCategory& dotationCategory ) const = 0;
    virtual bool   SupplyGetAvailableConvoyTransporter( PHY_ComposantePion*& pConvoyTransporter, MIL_AgentPion*& pConvoyTransporterPion, const PHY_ComposanteTypePion& transporterType ) const = 0;
    
    virtual MIL_AgentPion* SupplyCreateConvoyPion     ( const MIL_AgentTypePion& type, boost::shared_ptr< logistic::SupplyConvoyReal_ABC > convoy ) = 0;
    virtual void           SupplyDestroyConvoyPion    ( MIL_AgentPion& convoyPion ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void Serialize( sword::ParentEntity& message ) const = 0;
    //@}
};

} // end namespace logistic

#endif // __SupplySupplier_ABC_h_
