// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Dotations_h_
#define __PHY_RoleHLA_Dotations_h_

#include "PHY_RoleInterface_Dotations.h"

class MIL_Entity_ABC;

namespace dotation
{
// =============================================================================
/** @class  PHY_RoleHLA_Dotations
    @brief  HLA role dotations
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Dotations : public PHY_RoleInterface_Dotations
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoleHLA_Dotations( MIL_Entity_ABC& entity );
    virtual ~PHY_RoleHLA_Dotations();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetConsumptionMode() const;
    //@}

    virtual void ReadOverloading( xml::xistream& /*xis*/ ) {};

    virtual void NotifyCaptured() {};
    virtual void NotifyReleased() {};
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool /*bIsDead*/ ) {}
    virtual void Clean     () {}
    virtual bool HasChanged() const { return false; }
    virtual double GetDotationNumber( const PHY_DotationCategory & ) const { return 0.; }
    virtual bool HasIlluminationDotations( float /*range*/, bool /*permanent*/, PHY_DotationCategory& /*munition*/ ) const { return false; }
    //@}

    //! @name Dotations management
    //@{
    virtual double SupplyDotation     ( const PHY_DotationCategory& /*category*/, double /*rNbr*/ ) { return 0; }
    virtual void     ResupplyDotations  () {}
    virtual void     ResupplyDotations  ( const PHY_AmmoDotationClass& /*ammoDotationClass*/, double /*rFactor*/ ) {}
    virtual void     ResupplyDotations  ( const PHY_DotationType&      /*type*/             , double /*rFactor*/ ) {}
    //@}

    //! @name Consumption management
    //@{
    virtual bool                 SetConsumptionMode      ( const PHY_ConsumptionType& /*mode*/ ) { return false; }
    virtual void                 RollbackConsumptionMode () {}
    virtual double             GetMaxTimeForConsumption( const PHY_ConsumptionType& /*mode*/ ) const { return 0; }
    //@}

    //! @name Fire dotations
    //@{
    virtual double AddFireReservation( const PHY_DotationCategory& /*category*/, double /*rNbr*/ ) { return 0; }
    virtual const PHY_DotationCategory* GetIlluminationDotations( float /*range*/, bool /*permanent*/ ) const { return 0; }
    virtual float GetIlluminatingRange( ) const { return 0; }
    virtual void SetForbiddenDotation( const PHY_DotationCategory& /*category*/ ) {};
    virtual void RemoveForbiddenDotation( const PHY_DotationCategory& /*category*/ ) {};
    virtual void AllowAllDotations() {};
    //@}

    //! @name Network
    //@{
//    virtual void SendChangedState( client::UnitAttributes& asn ) const {};
//    virtual void SendFullState   ( client::UnitAttributes& asn ) const {};
    //@}

    //! @name Logistic - Supply
    //@{
    virtual void NotifySupplyNeeded          ( const PHY_DotationCategory& /*dotationCategory*/, bool /*bNewNeed*/ ) const {}; // Logistic
    virtual void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& /*supplyRequest*/ ) const {};
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& /*dotationType*/, const PHY_AmmoDotationClass* /*pAmmoDotationClass*/, double /*rCapacityFactor*/ ) const {};
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    //@}
};

} // namespace dotation

#endif // __PHY_RoleHLA_Dotations_h_
