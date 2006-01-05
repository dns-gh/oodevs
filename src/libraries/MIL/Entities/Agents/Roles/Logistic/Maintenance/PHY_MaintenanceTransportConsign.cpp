// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceTransportConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:58 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MaintenanceTransportConsign.h"
#include "PHY_RolePion_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"


BOOST_CLASS_EXPORT_GUID( PHY_MaintenanceTransportConsign, "PHY_MaintenanceTransportConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::PHY_MaintenanceTransportConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenance, composanteState )
{
    const PHY_Breakdown& breakdown = composanteState.GetComposanteBreakdown();

    if( breakdown.AffectMobility() )
        EnterStateWaitingForCarrier();
    else
        EnterStateGoingFrom();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::PHY_MaintenanceTransportConsign()
    : PHY_MaintenanceConsign_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceTransportConsign::~PHY_MaintenanceTransportConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceTransportConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceTransportConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceTransportConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eGoingFrom             :                                  EnterStateDiagnosing        (); break;
        case eWaitingForCarrier     : if( DoWaitingForCarrier() )      EnterStateCarrierGoingTo    (); break;
        case eCarrierGoingTo        :                                  EnterStateCarrierLoading    (); break;
        case eCarrierLoading        :                                  EnterStateCarrierGoingFrom  (); break;
        case eCarrierGoingFrom      :                                  EnterStateCarrierUnloading  (); break;
        case eCarrierUnloading      :                                  EnterStateDiagnosing        (); break;
        case eDiagnosing            :                                  ChooseStateAfterDiagnostic  (); break;
        case eSearchingForUpperLevel: if( DoSearchingForUpperLevel() ) EnterStateFinished          (); break; 
        case eFinished              :                                                                  break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
