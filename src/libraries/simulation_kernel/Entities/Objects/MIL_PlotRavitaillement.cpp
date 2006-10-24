//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PlotRavitaillement.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_PlotRavitaillement.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PlotRavitaillement.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_PlotRavitaillement, "MIL_PlotRavitaillement" )

//-----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PlotRavitaillement::MIL_PlotRavitaillement( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_PlotRavitaillement::MIL_PlotRavitaillement()
    : MIL_RealObject_ABC()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PlotRavitaillement::~MIL_PlotRavitaillement()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_PlotRavitaillement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PlotRavitaillement::ProcessAgentEntering
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_PlotRavitaillement::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    //assert( false );
    /*if( GetConstructionPercentage() < 1. )
        return;

    if ( agent.GetType().GetType() == eAgentTypeAutomate3D || agent.GetType().GetType() == eAgentTypePion3D )
        agent.GetUnit().DoMagicDotationResupplyFromTC2();
        */
}
