//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_BouchonMines.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: MIL_BouchonMines.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_BouchonMines.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_BouchonMines, "MIL_BouchonMines" )

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_BouchonMines::MIL_BouchonMines( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_BouchonMines::MIL_BouchonMines()
    : MIL_Obstacle()
{

}

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_BouchonMines::~MIL_BouchonMines()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive > 
void MIL_BouchonMines::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );

}

//=============================================================================
// EVENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_BouchonMines::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentMovingInside( agent );
    if( !IsBypassed() && CanInteractWith( agent ) )
        ApplyAttrition( agent );
}


