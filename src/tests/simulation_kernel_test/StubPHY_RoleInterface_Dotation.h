// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StubPHY_RoleInterface_Dotation_h_
#define __StubPHY_RoleInterface_Dotation_h_

#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include <xeumeuleu/xml.hpp>

class PHY_AmmoDotationClass;
class PHY_ConsumptionType;
class PHY_DotationCategory;
class PHY_DotationType;
class PHY_SupplyDotationRequestContainer;

class Stub_PHY_RoleInterface_Dotation : public dotation::PHY_RoleInterface_Dotations
{
public:
             Stub_PHY_RoleInterface_Dotation() {}
    virtual ~Stub_PHY_RoleInterface_Dotation() {}
    virtual const PHY_ConsumptionType& GetConsumptionMode() const { throw; }
    virtual void ReadOverloading( xml::xistream& /*xis*/ ) {}
    virtual void Update( bool /*bIsDead*/ ) {}
    virtual void Clean() {}
    virtual bool HasChanged() const { return false; }
    virtual double SupplyDotation( const PHY_DotationCategory& /*category*/, double /*rNbr*/ ) { return 1.; }
    virtual void ResupplyDotations( bool /*withLog*/ ) {}
    virtual void ResupplyDotations( const PHY_AmmoDotationClass& /*ammoDotationClass*/, double /*rFactor*/ ) {}
    virtual void ResupplyDotations( const PHY_DotationType& /*type*/, double /*rFactor*/ ) {}
    virtual bool SetConsumptionMode( const PHY_ConsumptionType& /*mode*/ ) { return false;}
    virtual void RollbackConsumptionMode() {}
    virtual double GetMaxTimeForConsumption( const PHY_ConsumptionType& /*mode*/ ) const { return 1.; }
    virtual double AddFireReservation( const PHY_DotationCategory& /*category*/, double /*rNbr*/ ) { return 1.; }
    virtual void NotifySupplyNeeded( const PHY_DotationCategory& /*dotationCategory*/, bool /*bNewNeed*/ ) const {}
    virtual void FillSupplyRequest( PHY_SupplyDotationRequestContainer& /*supplyRequest*/ ) const {}
    virtual void ChangeDotationsValueUsingTC2( const PHY_DotationType& /*dotationType*/, const PHY_AmmoDotationClass* /*pAmmoDotationClass*/, double /*rCapacityFactor*/ ) const {}
};

#endif // __StubPHY_RoleInterface_Dotation_h_
