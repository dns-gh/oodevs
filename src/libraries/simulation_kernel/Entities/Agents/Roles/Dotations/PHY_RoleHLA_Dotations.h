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
             PHY_RoleHLA_Dotations( MIL_Entity_ABC& entity );
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
    virtual void Update    ( bool /*bIsDead*/ ) {};
    virtual void Clean     () {};
    virtual bool HasChanged() const { return false; };
    //@}

    //! @name Dotations management
    //@{
    virtual MT_Float SupplyDotation     ( const PHY_DotationCategory& /*category*/, MT_Float /*rNbr*/ ) { return 0; };
    virtual void     ResupplyDotations  () {};
    virtual void     ResupplyDotations  ( const PHY_AmmoDotationClass& /*ammoDotationClass*/, MT_Float /*rFactor*/ ) {};
    virtual void     ResupplyDotations  ( const PHY_DotationType&      /*type*/             , MT_Float /*rFactor*/ ) {};
    //@}

    //! @name Consumption management
    //@{
    virtual bool                 SetConsumptionMode      ( const PHY_ConsumptionType& /*mode*/ ) { return false; };
    virtual void                 RollbackConsumptionMode () {};
    virtual MT_Float             GetMaxTimeForConsumption( const PHY_ConsumptionType& /*mode*/ ) const { return 0; };
    //@}

    //! @name Fire dotations
    //@{
    virtual MT_Float AddFireReservation( const PHY_DotationCategory& /*category*/, MT_Float /*rNbr*/ ) { return 0; };
    //@}

    //! @name Logistic - Supply
    //@{
    virtual void NotifySupplyNeeded          ( const PHY_DotationCategory& /*dotationCategory*/, bool /*bNewNeed*/ ) const {}; // Logistic
    virtual void FillSupplyRequest           ( PHY_SupplyDotationRequestContainer& /*supplyRequest*/ ) const {};
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& /*dotationType*/, const PHY_AmmoDotationClass* /*pAmmoDotationClass*/, MT_Float /*rCapacityFactor*/ ) const {};
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    //@}
};

} // namespace dotation

#endif // __PHY_RoleHLA_Dotations_h_
