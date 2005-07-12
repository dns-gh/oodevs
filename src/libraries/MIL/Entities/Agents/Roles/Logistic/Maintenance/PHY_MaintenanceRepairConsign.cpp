// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceRepairConsign.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:57 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceRepairConsign.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_MaintenanceRepairConsign.h"
#include "PHY_RolePion_Maintenance.h"
#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"

BOOST_CLASS_EXPORT_GUID( PHY_MaintenanceRepairConsign, "PHY_MaintenanceRepairConsign" )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : PHY_MaintenanceConsign_ABC( maintenance, composanteState )
{
    EnterStateWaitingForParts();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::PHY_MaintenanceRepairConsign()
    : PHY_MaintenanceConsign_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceRepairConsign::~PHY_MaintenanceRepairConsign()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceRepairConsign::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_MaintenanceConsign_ABC >( *this );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceRepairConsign::Update
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool PHY_MaintenanceRepairConsign::Update()
{
    if ( --nTimer_ > 0 )
        return GetState() == eFinished;

    switch( GetState() )
    {
        case eWaitingForParts         : if( DoWaitingForParts() )          EnterStateWaitingForRepairer      (); break;
        case eWaitingForRepairer      : if( DoWaitingForRepairer() )       EnterStateRepairing               (); break;
        case eRepairing               :                                    EnterStateWaitingForGoingBackToWar(); break;
        case eWaitingForGoingBackToWar: if( DoWaitingForGoingBackToWar() ) EnterStateGoingBackToWar          (); break;
        case eGoingBackToWar          :      DoReturnComposante();         EnterStateFinished                (); break;       
        case eFinished                :                                                                          break;
        default:
            assert( false );
    }
    return GetState() == eFinished;
}
