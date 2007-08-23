//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_SiteDecontamination.cpp $
// $Author: Nld $
// $Modtime: 26/04/05 11:54 $
// $Revision: 6 $
// $Workfile: MIL_SiteDecontamination.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_SiteDecontamination.h"
#include "MIL_RealObjectType.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

BOOST_CLASS_EXPORT_GUID( MIL_SiteDecontamination, "MIL_SiteDecontamination" )

//-----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_SiteDecontamination::MIL_SiteDecontamination( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC ( type, nID, army )
    , bHasDecontaminated_( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_SiteDecontamination::MIL_SiteDecontamination()
    : MIL_RealObject_ABC ()
    , bHasDecontaminated_( false ) 
{
}

//-----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_SiteDecontamination::~MIL_SiteDecontamination()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_SiteDecontamination::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


//=============================================================================
// EVENTS
//=============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination::ProcessAgentInside
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void MIL_SiteDecontamination::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );

    if( !CanInteractWith( agent ) )
        return;

    PHY_RoleInterface_NBC& roleNBC = agent.GetRole< PHY_RoleInterface_NBC >();

    if(    GetConstructionPercentage() < 1.
        || !roleNBC.IsContaminated()
        || bHasDecontaminated_ 
        || decontaminationQueue_.find( &agent ) == decontaminationQueue_.end() )
        return;

    if( GetType().GetNbrMaxAnimators() )
    {
        const MT_Float rRatioWorkers = (MT_Float)GetAnimators().size() / (MT_Float)GetType().GetNbrMaxAnimators();
        roleNBC.Decontaminate( rRatioWorkers );
        if( !roleNBC.IsContaminated() )
        {
            const T_AgentSet& animators = GetAnimators();
            for( CIT_AgentSet it = animators.begin(); it != animators.end(); ++it )
                MIL_Report::PostEvent( **it, MIL_Report::eReport_UnitDecontaminated );
        }
        bHasDecontaminated_ = true;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_SiteDecontamination::UpdateState
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void MIL_SiteDecontamination::UpdateState()
{
    MIL_RealObject_ABC::UpdateState();
    bHasDecontaminated_ = false;
    decontaminationQueue_.clear();
}