//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Installation.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_Installation.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_Installation.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_Installation, "MIL_Installation" )

//-----------------------------------------------------------------------------
// Name: MIL_Installation constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Installation::MIL_Installation()
    : MIL_RealObject_ABC( MIL_RealObjectType::installation_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_Installation destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Installation::~MIL_Installation()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Installation::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Installation::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
