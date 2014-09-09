// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Object_ABC.h"
#include "BuildableCapacity.h"
#include "FloodAttribute.h"
#include "MIL_Time_ABC.h"
#include "MIL_ObjectType_ABC.h"
#include "ObstacleAttribute.h"
#include "ResourceNetworkCapacity.h"
#include "SpawnCapacity.h"
#include "Decision/DEC_Gen_Object.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/ActivableCapacity.h"
#include "Entities/Objects/AltitudeModifierAttribute.h"
#include "Entities/Objects/InteractWithSideCapacity.h"
#include "Entities/Objects/LodgingAttribute.h"
#include "Entities/Objects/LogisticAttribute.h"
#include "Entities/Objects/MineAttribute.h"
#include "Entities/Objects/ObjectTypeResolver_ABC.h"
#include "Entities/Objects/TimeLimitedAttribute.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Objects/FireAttribute.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Tools/MIL_MessageParameters.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "protocol/Protocol.h"

MIL_IDManager MIL_Object_ABC::idManager_;

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: JSR 2011-01-19
// -----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned long forcedId )
    : id_                   ( idManager_.GetId( forcedId, true ) )
    , pArmy_                ( army )
    , pType_                ( &type )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
    , bNeedDestructionOnNextUpdate_( false )
{
    if( GetType().GetCapacity< SpawnCapacity >() )
        idManager_.GetId( forcedId + 1, true ); // we need to skip one ID for dynamic created object.
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC()
    : id_                   ( 0 )
    , pArmy_                ( 0 )
    , pType_                ( 0 )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
    , bNeedDestructionOnNextUpdate_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC destructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::~MIL_Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Register
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Register()
{
    if( pArmy_ )
        pArmy_->RegisterObject( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Unregister
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Unregister()
{
    if( pArmy_ )
        pArmy_->UnregisterObject( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Initialize
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Initialize( const TER_Localisation& localisation )
{
    TER_Object_ABC::Initialize( localisation, pType_->GetPointSize() );
    // Notify the agent natively inside the object that they are inside it
    TER_Agent_ABC::T_AgentPtrVector agentsInsideObject;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocalisation(), agentsInsideObject, GetPrecision() );
    for( auto itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyTerrainPutInsideObject( *this );
    // Notify object when population natively inside the object
    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector populationsInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( GetLocalisation(), populationsInsideObject, GetPrecision() );
    for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itPopulation = populationsInsideObject.begin(); itPopulation != populationsInsideObject.end(); ++itPopulation )
        NotifyPopulationMovingInside( static_cast< MIL_PopulationConcentration& >( **itPopulation ) );
    TER_PopulationFlow_ABC::T_PopulationFlowVector populationsFlowInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinLocalisation( GetLocalisation(), populationsFlowInsideObject, GetPrecision() );
    for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itPopulation = populationsFlowInsideObject.begin(); itPopulation != populationsFlowInsideObject.end(); ++itPopulation )
        NotifyPopulationMovingInside( static_cast< MIL_PopulationFlow& >( **itPopulation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Initialize
// Created: ABR 2012-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Initialize( const DEC_Gen_Object& genObject )
{
    ActivableCapacity* activableCapacity = Retrieve< ActivableCapacity >();
    if( activableCapacity && ( genObject.GetActivationTime() > 0 || genObject.GetMinesActivityTime() > 0 ) )
    {
        ObstacleAttribute& obstacleAttribute = GetAttribute< ObstacleAttribute >();
        obstacleAttribute.SetActivationTime( genObject.GetActivationTime() );
        obstacleAttribute.SetActivityTime( genObject.GetMinesActivityTime() );
    }
    const MIL_Automate* tc2 = genObject.GetTC2();
    if( tc2 )
    {
        MIL_AutomateLOG* logSuperior = tc2->GetBrainLogistic();
        if( logSuperior )
            GetAttribute< LogisticAttribute, LogisticAttribute >() = LogisticAttribute( *logSuperior );
    }
    if( genObject.GetAltitudeModifier() > 0 )
        GetAttribute< AltitudeModifierAttribute >().SetHeight( static_cast< unsigned int >( genObject.GetAltitudeModifier() ) );
    if( genObject.GetLodging() > 0 )
        GetAttribute< LodgingAttribute >().Update( genObject.GetLodging() );
    if( genObject.GetMining() )
        GetAttribute< MineAttribute >() .Set( 1. );
    if( genObject.GetTimeLimit() > 0 )
        GetAttribute< TimeLimitedAttribute >().SetLifeTime( genObject.GetTimeLimit() );
    const std::string& fireClass = genObject.GetFireClass();
    if( !fireClass.empty() )
        GetAttribute< FireAttribute >().SetFireClass( fireClass, genObject.GetMaxCombustion() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< TER_Object_ABC >( *this );
    file >> id_;
    idManager_.GetId( id_, true );
    std::string type;
    file >> type;
    pType_ = &file.GetObjectTypeResolver().FindType( type );
    file >> pArmy_;
    file >> bMarkedForDestruction_;
    file >> bReadyForDeletion_;
    file >> bNeedDestructionOnNextUpdate_;
    file >> interaction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< TER_Object_ABC >( *this );
    file << id_;
    file << pType_->GetName();
    file << pArmy_;
    file << bMarkedForDestruction_;
    file << bReadyForDeletion_;
    file << bNeedDestructionOnNextUpdate_;
    file << interaction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_Object_ABC::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// ----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanCollideWithEntity
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanCollideWithEntity() const
{    
    const ObstacleAttribute* obstacle = RetrieveAttribute< ObstacleAttribute >();
    if( obstacle && !obstacle->IsActivated() )
        return false;
    return !IsMarkedForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWith
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWith( const MIL_Agent_ABC& agent ) const
{
    const InteractWithSideCapacity* pSideInteraction = Retrieve< InteractWithSideCapacity >();
    if( pSideInteraction && pArmy_ )
        return pSideInteraction->IsPossible( *pArmy_, agent ) && CanCollideWithEntity();
    return CanCollideWithEntity();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWith
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWith( const MIL_Population& population ) const
{
    const InteractWithSideCapacity* pSideInteraction = Retrieve< InteractWithSideCapacity >();
    if( pSideInteraction && pArmy_ )
        return pSideInteraction->IsPossible( *pArmy_, population.GetArmy() ) && CanCollideWithEntity();
    return CanCollideWithEntity();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWithEntity
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWithEntity() const
{
    return CanCollideWithEntity();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyPopulationMovingInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population )
{
    if( !CanCollideWithEntity() )
        return;
    interaction_.NotifyPopulationMovingInside( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyPopulationMovingOutside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population )
{
    interaction_.NotifyPopulationMovingOutside( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentMovingInside( MIL_Agent_ABC& agent )
{
    if( !CanCollideWithEntity() )
        return;
    interaction_.NotifyAgentMovingInside( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentMovingOutside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentMovingOutside( MIL_Agent_ABC& agent )
{
    interaction_.NotifyAgentMovingOutside( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentPutInside
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentPutInside( MIL_Agent_ABC& agent )
{
    if( !CanCollideWithEntity() )
        return;
    interaction_.NotifyAgentPutInside( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyAgentPutOutside
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyAgentPutOutside( MIL_Agent_ABC& agent )
{
    interaction_.NotifyAgentPutOutside( agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::UpdateState
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_Object_ABC::UpdateState()
{
    if( IsMarkedForDestruction() )
        bReadyForDeletion_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::ProcessEvents
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_Object_ABC::ProcessEvents()
{
    Update( MIL_Time_ABC::GetTime().GetCurrentTimeStep() );
    interaction_.ProcessInteractionEvents( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Clean
// Created: JSR 2013-01-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Clean()
{
    interaction_.Clean( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::UpdateLocalisation
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void MIL_Object_ABC::UpdateLocalisation( const TER_Localisation& newLocalisation )
{
    TER_Object_ABC::UpdateLocalisation( newLocalisation );
    interaction_.UpdateInteraction( *this, GetLocalisation() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsInside
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::IsInside( const MT_Vector2D& vPos ) const
{
    if( const FloodAttribute* flood = RetrieveAttribute< FloodAttribute >() )
        return flood->GetLocalisation().IsInside( vPos );
    return TER_Object_ABC::IsInside( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsOnBorder
// Created: CMA 2012-03-14
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::IsOnBorder( const MT_Vector2D& vPos ) const
{
    if( const FloodAttribute* flood = RetrieveAttribute< FloodAttribute >() )
        return flood->GetLocalisation().IsOnBorder( vPos );
    return TER_Object_ABC::IsOnBorder( vPos );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::MarkForDestruction
// Created: NLD 2004-07-01
// -----------------------------------------------------------------------------
void MIL_Object_ABC::MarkForDestruction()
{
    bMarkedForDestruction_ = true;
    bNeedDestructionOnNextUpdate_ = false;
    interaction_.ClearInteraction( *this );
    TER_Object_ABC::Terminate(); // Degueu : vire l'objet du monde
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::MarkForDestructionNextUpdate
// Created: LDC 2014-05-05
// -----------------------------------------------------------------------------
void MIL_Object_ABC::MarkForDestructionNextUpdate()
{
    bNeedDestructionOnNextUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::OnUpdateResourceLinks
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void MIL_Object_ABC::OnUpdateResourceLinks( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list )
{
    ResourceNetworkCapacity* capacity = Retrieve< ResourceNetworkCapacity >();
    protocol::Check( capacity, "object has no resource network capacity" );
    capacity->Update( list );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsMarkedForDestruction
// Created: NLD 2003-10-10
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::IsMarkedForDestruction() const
{
    return bMarkedForDestruction_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsMarkedForDestructionNextUpdate
// Created: LDC 2014-05-05
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::IsMarkedForDestructionNextUpdate() const
{
    return bNeedDestructionOnNextUpdate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::IsReadyForDeletion
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::IsReadyForDeletion() const
{
    return bReadyForDeletion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MIL_Army_ABC* MIL_Object_ABC::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Army_ABC* MIL_Object_ABC::GetArmy()
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::GetID
// Created: BCI 2011-01-12
// -----------------------------------------------------------------------------
unsigned int MIL_Object_ABC::GetID() const
{
    return id_;
}
