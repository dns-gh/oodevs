//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_AirePoser.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 5 $
// $Workfile: MIL_AirePoser.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AirePoser.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_AirePoser, "MIL_AirePoser" )

//-----------------------------------------------------------------------------
// Name: MIL_AirePoser constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_AirePoser::MIL_AirePoser()
    : MIL_RealObject_ABC( MIL_RealObjectType::airePoser_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_AirePoser destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_AirePoser::~MIL_AirePoser()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_AirePoser::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_AirePoser::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
