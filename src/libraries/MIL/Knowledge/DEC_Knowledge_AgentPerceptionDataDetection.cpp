// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataDetection.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:57 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataDetection.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_AgentPerceptionDataDetection.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataDetection::DEC_Knowledge_AgentPerceptionDataDetection()
    : nTimeLastUpdate_             ( 0 )
    , rAltitude_                   ( 0. )
    , rSpeed_                      ( 0. )
    , pLastPosture_                ( 0 )
    , pCurrentPosture_             ( 0 )
    , rPostureCompletionPercentage_( 1. )
    , bSurrendered_                ( false )
    , bPrisoner_                   ( false ) 
    , bRefugeeManaged_             ( false )
{

}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataDetection::~DEC_Knowledge_AgentPerceptionDataDetection()
{
}


// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> nTimeLastUpdate_
         >> vPosition_
         >> vDirection_
         >> rAltitude_
         >> rSpeed_
         >> bSurrendered_
         >> bPrisoner_
         >> bRefugeeManaged_
         >> rPostureCompletionPercentage_;
         
    // Desérialisation des volumes par nom ( données "statiques" )
    uint nNbr;
    uint nID;

    file >> nNbr;
    while ( nNbr-- )
    {
        file >> nID;
        visionVolumes_.insert( PHY_Volume::FindVolume( nID ) );
    }

    // Déserialisation des postures ( données statiques )
    file >> nID;
    pLastPosture_    = PHY_Posture::FindPosture( nID );
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nTimeLastUpdate_
         << vPosition_
         << vDirection_
         << rAltitude_
         << rSpeed_
         << bSurrendered_
         << bPrisoner_
         << bRefugeeManaged_
         << rPostureCompletionPercentage_;
         
    // Serialisation des volumes par nom ( données "statiques" )
    file << visionVolumes_.size();
    for ( CIT_ComposanteVolumeSet it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
        file << (*it)->GetID();

    // Serialisation des postures ( données statiques )
    file << ( pLastPosture_    ? pLastPosture_->GetID()    : (uint)-1 );
    file << ( pCurrentPosture_ ? pCurrentPosture_->GetID() : (uint)-1 );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::Prepare
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::Prepare( const MIL_Agent_ABC& agentPerceived )
{
    const PHY_RoleInterface_Location& roleLocation = agentPerceived.GetRole< PHY_RoleInterface_Location >();

    vPosition_  = roleLocation.GetPosition();
    rAltitude_  = roleLocation.GetAltitude();

    agentPerceived.GetRole< PHY_RoleInterface_Composantes >().GetVisibleVolumes( visionVolumes_ );

    const PHY_RoleInterface_Posture& rolePosture = agentPerceived.GetRole< PHY_RoleInterface_Posture >();
    pLastPosture_                 = &rolePosture.GetLastPosture   ();
    pCurrentPosture_              = &rolePosture.GetCurrentPosture();
    rPostureCompletionPercentage_ =  rolePosture.GetPostureCompletionPercentage();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel < PHY_PerceptionLevel::detected_ )
        return;

    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nTimeLastUpdate_ >= nCurrentTimeStep )
        return;
    nTimeLastUpdate_ = nCurrentTimeStep;

    Prepare( agentPerceived );

    const PHY_RoleInterface_Location& roleLocation = agentPerceived.GetRole< PHY_RoleInterface_Location >();
    vDirection_ = roleLocation.GetDirection   ();
    rSpeed_     = roleLocation.GetCurrentSpeed();

    const PHY_RoleInterface_Surrender& roleSurrender = agentPerceived.GetRole< PHY_RoleInterface_Surrender >();
    bSurrendered_ = roleSurrender.IsSurrendered();
    bPrisoner_    = roleSurrender.IsPrisoner   ();

    bRefugeeManaged_ = agentPerceived.GetRole< PHY_RoleInterface_Refugee >().IsManaged();
}
