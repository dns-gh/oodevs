//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PlateForme.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 5 $
// $Workfile: MIL_PlateForme.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_PlateForme.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_PlateForme, "MIL_PlateForme" )

//-----------------------------------------------------------------------------
// Name: MIL_PlateForme constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PlateForme::MIL_PlateForme()
    : MIL_RealObject_ABC( MIL_RealObjectType::plateForme_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_PlateForme destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_PlateForme::~MIL_PlateForme()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_PlateForme::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_PlateForme::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
