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

#include "MIL_Pch.h"

#include "MIL_BouchonMines.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_BouchonMines, "MIL_BouchonMines" )

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_BouchonMines::MIL_BouchonMines()
    : MIL_Obstacle( MIL_RealObjectType::bouchonMines_ )
{
    nFullNbrDotationForConstruction_ = 10; // Nbr mines
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
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_BouchonMines::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    rSizeCoef_                       = diaParameters[ nCurrentParamIdx ].ToFloat();  // Nbr mines
    nFullNbrDotationForConstruction_ = (uint)rSizeCoef_;
    return true;
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
    if( IsBypassed() )
        return;

    ApplyAttrition( agent );
}

