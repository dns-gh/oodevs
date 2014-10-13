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
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/MIL_Army_ABC.h"
#include "MIL_Time_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentPerceptionDataDetection )

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
    , rPopulationDensity_          ( 0. )
    , rOperationalState_           ( 1. )
    , pArmySurrenderedTo_          ( 0 )
    , pArmy_                       ( 0 )
    , bIsPC_                       ( false )
    , bPrisoner_                   ( false )
    , bRefugeeManaged_             ( false )
    , bDead_                       ( false )
    , bWounded_                    ( false )
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
void DEC_Knowledge_AgentPerceptionDataDetection::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> nTimeLastUpdate_;
    file >> vPosition_;
    file >> vDirection_;
    file >> rAltitude_;
    file >> rSpeed_;
    file >> rPopulationDensity_;
    file >> rOperationalState_;
    file >> const_cast< MIL_Army_ABC*& >( pArmySurrenderedTo_ );
    file >> rPostureCompletionPercentage_;
    file >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    file >> bIsPC_;
    file >> bDead_;
    file >> bPrisoner_;
    file >> bRefugeeManaged_;
    file >> bWounded_;
    std::size_t nNbr;
    unsigned int nID;
    file >> nNbr;
    while( nNbr-- )
    {
        file >> nID;
        const PHY_Volume* volume = PHY_Volume::FindVolume( nID );
        if( volume )
            visionVolumes_.push_back( volume );
    }
    file >> nID;
    pLastPosture_ = PHY_Posture::FindPosture( nID );
    file >> nID;
    pCurrentPosture_ = PHY_Posture::FindPosture( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nTimeLastUpdate_;
    file << vPosition_;
    file << vDirection_;
    file << rAltitude_;
    file << rSpeed_;
    file << rPopulationDensity_;
    file << rOperationalState_;
    file << pArmySurrenderedTo_;
    file << rPostureCompletionPercentage_;
    file << pArmy_;
    file << bIsPC_;
    file << bDead_;
    file << bPrisoner_;
    file << bRefugeeManaged_;
    file << bWounded_;
    std::size_t size = visionVolumes_.size();
    for( auto it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
        if( !( *it ) )
            --size;
    file << size;
    for( auto it = visionVolumes_.begin(); it != visionVolumes_.end(); ++it )
    {
        if( (*it) )
        {
            unsigned int id = (*it)->GetID();
            file << id;
        }
    }
    unsigned int last = ( pLastPosture_ ? pLastPosture_->GetID() : static_cast< unsigned int >( -1 ) );
    unsigned int current = ( pCurrentPosture_ ? pCurrentPosture_->GetID() : static_cast< unsigned int >( -1 ) );
    file << last;
    file << current;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::Prepare
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::Prepare( const MIL_Agent_ABC& agentPerceived )
{
    const PHY_RoleInterface_Location& roleLocation = agentPerceived.GetRole< PHY_RoleInterface_Location >();
    vPosition_ = roleLocation.GetPosition();
    rAltitude_ = roleLocation.GetAltitude();
    visionVolumes_ = agentPerceived.GetRole< PHY_RoleInterface_Composantes >().GetVisibleVolumes();
    const PHY_RoleInterface_Posture& rolePosture = agentPerceived.GetRole< PHY_RoleInterface_Posture >();
    pLastPosture_ = &rolePosture.GetLastPosture   ();
    pCurrentPosture_ = &rolePosture.GetCurrentPosture();
    rPostureCompletionPercentage_ =  rolePosture.GetPostureCompletionPercentage();
    rPopulationDensity_ = agentPerceived.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataDetection::Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel < PHY_PerceptionLevel::detected_ )
        return;
    const unsigned int nCurrentTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( nTimeLastUpdate_ >= nCurrentTimeStep )
        return;
    nTimeLastUpdate_ = nCurrentTimeStep;
    Prepare( agentPerceived );
    const PHY_RoleInterface_Location& roleLocation = agentPerceived.GetRole< PHY_RoleInterface_Location >();
    vDirection_ = roleLocation.GetDirection   ();
    rSpeed_ = roleLocation.GetCurrentSpeed();
    const surrender::PHY_RoleInterface_Surrender& roleSurrender = agentPerceived.GetRole< surrender::PHY_RoleInterface_Surrender >();
    pArmySurrenderedTo_ = roleSurrender.GetArmySurrenderedTo();
    bPrisoner_ = roleSurrender.IsPrisoner();
    bRefugeeManaged_ = agentPerceived.GetRole< PHY_RoleInterface_Refugee >().IsManaged();
    bDead_ = agentPerceived.IsDead();
    bWounded_ = agentPerceived.GetRole< human::PHY_RoleInterface_Humans >().HasWoundedHumansToEvacuate();
    pArmy_ = &agentPerceived.GetArmy();
    bIsPC_ = agentPerceived.IsPC();
    rOperationalState_ = agentPerceived.GetRole< PHY_RoleInterface_Composantes >().GetOperationalState();
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
unsigned int DEC_Knowledge_AgentPerceptionDataDetection::GetTimeLastUpdate() const
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
double DEC_Knowledge_AgentPerceptionDataDetection::GetSpeed() const
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
double DEC_Knowledge_AgentPerceptionDataDetection::GetAltitude() const
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
double DEC_Knowledge_AgentPerceptionDataDetection::GetPostureCompletionPercentage() const
{
    return rPostureCompletionPercentage_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetVisibleVolumes
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentPerceptionDataDetection::T_ComposanteVolumes& DEC_Knowledge_AgentPerceptionDataDetection::GetVisibleVolumes() const
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
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetArmy
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentPerceptionDataDetection::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsPC
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataDetection::IsPC() const
{
    return bIsPC_;
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetPopulationDensity
// Created: LDC 2011-04-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentPerceptionDataDetection::GetPopulationDensity() const
{
    return rPopulationDensity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::IsWounded
// Created: LGY 2012-12-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataDetection::IsWounded() const
{
    return bWounded_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataDetection::GetOperationalState
// Created: JSR 2014-07-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentPerceptionDataDetection::GetOperationalState() const
{
    return rOperationalState_;
}
