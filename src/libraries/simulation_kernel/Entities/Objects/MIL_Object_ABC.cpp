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
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "simulation_terrain/TER_World.h"
#include "MIL_ObjectType_ABC.h"
#include "MIL_ObjectFactory.h"

#include "Entities/MIL_Army.h"

using namespace hla;

//-----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: NLD 2002-12-12
//-----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC( MIL_Army_ABC& army, const MIL_ObjectType_ABC& type )
    : pArmy_                ( &army )
    , pType_                ( &type )
    , bMarkedForDestruction_( false )
    , bReadyForDeletion_    ( false )
{  
    // NOTHING  
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC constructor
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
MIL_Object_ABC::MIL_Object_ABC()
    : pArmy_                ( 0 )
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
    if ( !bMarkedForDestruction_ ) // Should already be done
        MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Register
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Register()
{
    pArmy_->RegisterObject( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Unregister
// Created: LDC 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Unregister()
{
    pArmy_->UnregisterObject( *this );
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::Initialize
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
void MIL_Object_ABC::Initialize( const TER_Localisation& localisation )
{
    TER_Object_ABC::Initialize( localisation );    

    // Notify the agent natively inside the object that they are inside it
    TER_Agent_ABC::T_AgentPtrVector agentsInsideObject;
    TER_World::GetWorld().GetAgentManager().GetListWithinLocalisation( GetLocalisation(), agentsInsideObject );
    for( TER_Agent_ABC::CIT_AgentPtrVector itAgent = agentsInsideObject.begin(); itAgent != agentsInsideObject.end(); ++itAgent )
        static_cast< PHY_RoleInterface_Location& >( **itAgent ).NotifyTerrainPutInsideObject( *this );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_Object_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< TER_Object_ABC >( *this );
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
void MIL_Object_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< TER_Object_ABC >( *this );
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

//=============================================================================
// TOOLS
//=============================================================================

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

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyPopulationMovingInside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population )
{
    if( !CanCollideWithEntity() )
        return;
    interaction_.NotifyAgentMovingInside( population );
}


// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::NotifyPopulationMovingOutside
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
void MIL_Object_ABC::NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population )
{
    interaction_.NotifyAgentMovingOutside( population );
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

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Object_ABC::UpdateState
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void MIL_Object_ABC::UpdateState()
{
    if( bMarkedForDestruction_ )
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

// =============================================================================
// TOOLS
// =============================================================================

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
// Name: MIL_Object_ABC::MarkForDestruction
// Created: NLD 2004-07-01
// -----------------------------------------------------------------------------
void MIL_Object_ABC::MarkForDestruction()
{
    bMarkedForDestruction_ = true;
    interaction_.ClearInteraction( *this );    
    TER_Object_ABC::Terminate(); // Degueu : vire l'objet du monde
}
