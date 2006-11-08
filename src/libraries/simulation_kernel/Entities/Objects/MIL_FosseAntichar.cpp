//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FosseAntichar.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:45 $
// $Revision: 7 $
// $Workfile: MIL_FosseAntichar.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_FosseAntiChar.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_FosseAntiChar, "MIL_FosseAntiChar" )

//-----------------------------------------------------------------------------
// Name: MIL_FosseAntiChar constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FosseAntiChar::MIL_FosseAntiChar( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_FosseAntiChar constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_FosseAntiChar::MIL_FosseAntiChar()
    : MIL_Obstacle()
{

}

//-----------------------------------------------------------------------------
// Name: MIL_FosseAntiChar destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FosseAntiChar::~MIL_FosseAntiChar()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_FosseAntiChar::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_FosseAntiChar::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

