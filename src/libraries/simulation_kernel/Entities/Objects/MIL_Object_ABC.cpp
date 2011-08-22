 //*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Object_ABC.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 15:21 $
// $Revision: 7 $
// $Workfile: MIL_Object_ABC.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Object_ABC.h"
#include "MIL_ObjectInteraction.h"
#include "MIL_AgentServer.h"
#include "MIL_ObjectFactory.h"
#include "MIL_ObjectType_ABC.h"
#include "FloodAttribute.h"
#include "ResourceNetworkCapacity.h"
#include "SpawnCapacity.h"
#include "StructuralCapacity.h"
#include "UniversalCapacity.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/MIL_Army.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "protocol/Protocol.h"
#include <boost/serialization/vector.hpp>

MIL_IDManager MIL_Object_ABC::idManager_;

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type )
    : id_                   ( idManager_.GetFreeId() )
    , pArmy_                ( army )
    , pType_                ( &type )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
{
    if( GetType().GetCapacity< SpawnCapacity >() )
        idManager_.GetFreeId(); // we need to skip one ID for dynamic created object.
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: JSR 2011-01-19
// -----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC( MIL_Army_ABC* army, const MIL_ObjectType_ABC& type, unsigned int id )
    : id_                   ( id )
    , pArmy_                ( army )
    , pType_                ( &type )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
{
    idManager_.Lock( id_ );
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
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC destructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::~MIL_Object_ABC()
{
    if( !bMarkedForDestruction_ ) // Should already be done
        MarkForDestruction();
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
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocalisation(), agentsInsideObject );
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyTerrainPutInsideObject( *this );
    // Notify object when population natively inside the object
    TER_PopulationConcentration_ABC::T_PopulationConcentrationVector populationsInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetConcentrationManager().GetListWithinLocalisation( GetLocalisation(), populationsInsideObject );
    for( TER_PopulationConcentration_ABC::CIT_PopulationConcentrationVector itPopulation = populationsInsideObject.begin(); itPopulation != populationsInsideObject.end(); ++itPopulation )
        NotifyPopulationMovingInside( static_cast< MIL_PopulationConcentration& >( **itPopulation ) );
    TER_PopulationFlow_ABC::T_PopulationFlowVector populationsFlowInsideObject;
    TER_World::GetWorld().GetPopulationManager().GetFlowManager().GetListWithinLocalisation( GetLocalisation(), populationsFlowInsideObject );
    for( TER_PopulationFlow_ABC::CIT_PopulationFlowVector itPopulation = populationsFlowInsideObject.begin(); itPopulation != populationsFlowInsideObject.end(); ++itPopulation )
        NotifyPopulationMovingInside( static_cast< MIL_PopulationFlow& >( **itPopulation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< TER_Object_ABC >( *this )
         >> id_;
    idManager_.Lock( id_ );
    std::string type;
    file >> type;
    pType_ = &MIL_ObjectFactory::FindType( type );
    file >> pArmy_
         >> bMarkedForDestruction_
         >> bReadyForDeletion_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< TER_Object_ABC >( *this )
         << id_;
    file << pType_->GetName();
    file << pArmy_
         << bMarkedForDestruction_
         << bReadyForDeletion_;
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
    return !IsMarkedForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWith
// Created: NLD 2005-09-08
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWith( const MIL_Agent_ABC& /*agent*/ ) const
{
    return CanCollideWithEntity();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::CanInteractWith
// Created: NLD 2005-12-10
// -----------------------------------------------------------------------------
bool MIL_Object_ABC::CanInteractWith( const MIL_PopulationElement_ABC& /*population*/ ) const
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
    Update( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() );
    interaction_.ProcessInteractionEvents( *this );
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
// Name: MIL_Object_ABC::MarkForDestruction
// Created: NLD 2004-07-01
// -----------------------------------------------------------------------------
void MIL_Object_ABC::MarkForDestruction()
{
    bMarkedForDestruction_ = true;
    interaction_.ClearInteraction( *this );
    TER_Object_ABC::Terminate(); // Degueu : vire l'objet du monde
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::OnUpdateResourceLinks
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
sword::MagicActionAck_ErrorCode MIL_Object_ABC::OnUpdateResourceLinks( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list )
{
    ResourceNetworkCapacity* capacity = Retrieve< ResourceNetworkCapacity >();
    if( !capacity )
        return sword::MagicActionAck::error_invalid_parameter;
    capacity->Update( list );
    return sword::MagicActionAck::no_error;
}
