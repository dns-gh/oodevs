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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentPerceptionDataDetection.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/MIL_Army_ABC.h"
#include "MIL_AgentServer.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentPerceptionDataDetection, "DEC_Knowledge_AgentPerceptionDataDetection" )

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
    , pArmySurrenderedTo_          ( false )
    , bPrisoner_                   ( false ) 
    , bRefugeeManaged_             ( false )
    , bDead_                       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataDetection::~DEC_Knowledge_AgentPerceptionDataDetection()
{
    // NOTHING
}

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
         >> const_cast< MIL_Army_ABC*& >( pArmySurrenderedTo_ )
         >> bPrisoner_
         >> bRefugeeManaged_
         >> bDead_
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
         << pArmySurrenderedTo_
         << bPrisoner_
         << bRefugeeManaged_
         << bDead_
         << rPostureCompletionPercentage_;
         
    // Serialisation des volumes par nom ( données "statiques" )
    unsigned size = visionVolumes_.size();
    file << size;
    for ( CIT_ComposanteVolumeSet it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
    {
        unsigned id = (*it)->GetID();
        file << id;
    }

    // Serialisation des postures ( données statiques )
    unsigned last    = ( pLastPosture_    ? pLastPosture_->GetID()    : (uint)-1 ),
             current = ( pCurrentPosture_ ? pCurrentPosture_->GetID() : (uint)-1 );
    file << last << current;
}

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

    const surrender::PHY_RoleInterface_Surrender& roleSurrender = agentPerceived.GetRole< surrender::PHY_RoleInterface_Surrender >();
    pArmySurrenderedTo_ = roleSurrender.GetArmySurrenderedTo();
    bPrisoner_          = roleSurrender.IsPrisoner          ();

    bRefugeeManaged_ = agentPerceived.GetRole< PHY_RoleInterface_Refugee >().IsManaged();
    bDead_           = agentPerceived.IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsDead
// Created: NLD 2006-02-01
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataDetection::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetTimeLastUpdate
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
uint DEC_Knowledge_AgentPerceptionDataDetection::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetDirection
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentPerceptionDataDetection::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetSpeed
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetSpeed() const
{
    return rSpeed_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetPosition
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_AgentPerceptionDataDetection::GetPosition() const
{
    return vPosition_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetAltitude
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetAltitude() const
{
    return rAltitude_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetLastPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentPerceptionDataDetection::GetLastPosture() const
{
    assert( pLastPosture_ );
    return *pLastPosture_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetCurrentPosture
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_AgentPerceptionDataDetection::GetCurrentPosture() const
{
    assert( pCurrentPosture_ );
    return *pCurrentPosture_;
}
          
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetPostureCompletionPercentage
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentPerceptionDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetVisionVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const T_ComposanteVolumeSet& DEC_Knowledge_AgentPerceptionDataDetection::GetVisionVolumes() const
{
    return visionVolumes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetArmySurrenderedTo
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentPerceptionDataDetection::GetArmySurrenderedTo() const
{
    return pArmySurrenderedTo_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsPrisoner
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataDetection::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataDetection::IsRefugeeManaged() const
{
    return bRefugeeManaged_;
}
