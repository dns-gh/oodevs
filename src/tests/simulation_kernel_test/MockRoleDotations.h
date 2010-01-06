// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleDotations_h_
#define __MockRoleDotations_h_

#include "simulation_kernel/Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"

// =============================================================================
/** @class  MockRoleDotations
    @brief  MockRoleDotations
*/
// Created: LDC 2010-01-04
// =============================================================================
class MockRoleDotations
    : public mockpp::ChainableMockObject
    , public dotation::PHY_RoleInterface_Dotations
{

public:
    //! @name Constructors/Destructor
    //@{
    MockRoleDotations()
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockRoleDotations" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ReadOverloadingShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Update )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Clean )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ResupplyDotationsShadow1 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ResupplyDotationsShadow2 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ResupplyDotationsShadow3 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SetConsumptionModeShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RollbackConsumptionMode )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( NotifySupplyNeededShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( FillSupplyRequestShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ChangeDotationsValueUsingTC2Shadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetConsumptionModeShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SupplyDotationShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( AddFireReservationShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( HasChanged )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetMaxTimeForConsumptionShadow )
    {}
    virtual ~MockRoleDotations() {}
    //@}

    virtual const PHY_ConsumptionType& GetConsumptionMode() const
    {
        return *GetConsumptionModeShadow();
    }
    virtual void ReadOverloading( xml::xistream& xis )
    {
        const xml::xistream* pXis = &xis;
        ReadOverloadingShadow( pXis );
    }
    virtual void ResupplyDotations()
    {
        ResupplyDotationsShadow1();
    }
    virtual void ResupplyDotations( const PHY_AmmoDotationClass& ammoDotationClass, double rFactor )
    {
        const PHY_AmmoDotationClass* pAmmoDotationClass = &ammoDotationClass;
        ResupplyDotationsShadow2( pAmmoDotationClass, rFactor );
    }
    virtual void ResupplyDotations( const PHY_DotationType& type, double rFactor )
    {
        const PHY_DotationType* pType = &type;
        ResupplyDotationsShadow3( pType, rFactor );
    }
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& dotationCategory, bool bNewNeed ) const
    {
        const PHY_DotationCategory* pDotationCategory = &dotationCategory;
        NotifySupplyNeededShadow( pDotationCategory, bNewNeed );
    }
    virtual void FillSupplyRequest( PHY_SupplyDotationRequestContainer& supplyRequest ) const
    {
        const PHY_SupplyDotationRequestContainer* pSupplyRequest = &supplyRequest;
        FillSupplyRequestShadow( pSupplyRequest );
    }
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& dotationType, const PHY_AmmoDotationClass* pAmmoDotationClass, double rCapacityFactor ) const
    {
        const PHY_DotationType* pDotationType = &dotationType;
        ChangeDotationsValueUsingTC2Shadow( pDotationType, pAmmoDotationClass, rCapacityFactor );
    }
    virtual double SupplyDotation( const PHY_DotationCategory& category, double rNbr )
    {
        const PHY_DotationCategory* pCategory = &category;
        return SupplyDotationShadow( pCategory, rNbr );
    }
    virtual double AddFireReservation( const PHY_DotationCategory& category, double rNbr )
    {
        const PHY_DotationCategory* pCategory = &category;
        return AddFireReservationShadow( pCategory, rNbr );
    }
    virtual double GetMaxTimeForConsumption( const PHY_ConsumptionType& mode ) const
    {
        const PHY_ConsumptionType* pMode = &mode;
        return GetMaxTimeForConsumptionShadow( pMode );
    }
    virtual bool SetConsumptionMode( const PHY_ConsumptionType& mode )
    {
        const PHY_ConsumptionType* pMode = &mode;
        return SetConsumptionModeShadow( pMode );
    }

    MOCKPP_VOID_CHAINABLE1( MockRoleDotations, ReadOverloadingShadow, xml::xistream* );    
    MOCKPP_VOID_CHAINABLE_EXT1( MockRoleDotations, Update, bool, , bool );
    MOCKPP_VOID_CHAINABLE0( MockRoleDotations, Clean );
    MOCKPP_VOID_CHAINABLE0( MockRoleDotations, ResupplyDotationsShadow1 );
    MOCKPP_VOID_CHAINABLE2( MockRoleDotations, ResupplyDotationsShadow2, const PHY_AmmoDotationClass*, double );
    MOCKPP_VOID_CHAINABLE2( MockRoleDotations, ResupplyDotationsShadow3, const PHY_DotationType*, double );
    MOCKPP_CHAINABLE1( MockRoleDotations, bool, SetConsumptionModeShadow, const PHY_ConsumptionType* );
    MOCKPP_VOID_CHAINABLE0( MockRoleDotations, RollbackConsumptionMode );

    MOCKPP_VOID_CONST_CHAINABLE2( MockRoleDotations, NotifySupplyNeededShadow, const PHY_DotationCategory*, bool );
    MOCKPP_VOID_CONST_CHAINABLE1( MockRoleDotations, FillSupplyRequestShadow, PHY_SupplyDotationRequestContainer* );
    MOCKPP_VOID_CONST_CHAINABLE3( MockRoleDotations, ChangeDotationsValueUsingTC2Shadow, const PHY_DotationType*, const PHY_AmmoDotationClass*, double );
    
    MOCKPP_CONST_CHAINABLE_EXT0( MockRoleDotations, const PHY_ConsumptionType*, GetConsumptionModeShadow, PHY_ConsumptionType, ); 
    MOCKPP_CHAINABLE2( MockRoleDotations, double, SupplyDotationShadow, const PHY_DotationCategory*, double );
    MOCKPP_CHAINABLE2( MockRoleDotations, double, AddFireReservationShadow, const PHY_DotationCategory*, double );
    MOCKPP_CONST_CHAINABLE0( MockRoleDotations, bool, HasChanged );
    MOCKPP_CONST_CHAINABLE1( MockRoleDotations, double, GetMaxTimeForConsumptionShadow, const PHY_ConsumptionType* );
};

#endif // __MockRoleDotation_h_
