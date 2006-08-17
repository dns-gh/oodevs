//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMobiliteAmelioree.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_ZoneMobiliteAmelioree.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_ZoneMobiliteAmelioree.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneMobiliteAmelioree, "MIL_ZoneMobiliteAmelioree" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMobiliteAmelioree constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMobiliteAmelioree::MIL_ZoneMobiliteAmelioree()
    : MIL_RealObject_ABC( MIL_RealObjectType::zoneMobiliteAmelioree_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMobiliteAmelioree destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMobiliteAmelioree::~MIL_ZoneMobiliteAmelioree()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneMobiliteAmelioree::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneMobiliteAmelioree::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
