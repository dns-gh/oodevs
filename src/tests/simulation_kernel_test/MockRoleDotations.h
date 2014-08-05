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

MOCK_BASE_CLASS( MockRoleDotations, dotation::PHY_RoleInterface_Dotations )
{
    MOCK_METHOD( GetConsumptionMode, 0 );
    MOCK_METHOD( ReadOverloading, 1 );
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( HasChanged, 0 );

    MOCK_METHOD( SupplyDotation, 2 );
    MOCK_METHOD( ResupplyDotations, 1, void( bool ), ResupplyDotationsAll );
    MOCK_METHOD( ResupplyDotations, 2, void( const PHY_AmmoDotationClass&, double ), ResupplyDotationsByAmmo );
    MOCK_METHOD( ResupplyDotations, 2, void( const PHY_DotationType&, double ), ResupplyDotationsByType );
    MOCK_METHOD( ChangeDotation, 2, void( const PHY_DotationCategory&, double ), ChangeDotation );
    MOCK_METHOD( ChangeDotation, 2, void( const PHY_DotationType&, double ), ChangeDotationByType );

    MOCK_METHOD( SetConsumptionMode, 1 );
    MOCK_METHOD( RollbackConsumptionMode, 0 );
    MOCK_METHOD( GetMaxTimeForConsumption, 1 );

    MOCK_METHOD( AddFireReservation, 2 );
    MOCK_METHOD( GetDotationNumber, 1 );
    MOCK_METHOD( GetFuelDotationNumber, 0 );
    MOCK_METHOD( GetFuelDotationCapacity, 0 );
    MOCK_METHOD( GetIlluminationDotations, 2 );
    MOCK_METHOD( SetForbiddenDotation, 1 );
    MOCK_METHOD( RemoveForbiddenDotation, 1 );
    MOCK_METHOD( AllowAllDotations, 0 );
    MOCK_METHOD( HasDotationForFiring, 2 );

    MOCK_METHOD( NotifySupplyNeeded, 2 );
    MOCK_METHOD( Apply, 1 );
    MOCK_METHOD( ChangeDotationsValueUsingTC2, 3 );
    MOCK_METHOD( EnforceAviationResources, 1 );
    MOCK_METHOD( GetIlluminatingRange, 0 );
    MOCK_METHOD( GetPion, 0 );
};

#endif // __MockRoleDotation_h_
