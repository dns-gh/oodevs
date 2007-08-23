//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZonePoserHelicoptere.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_ZonePoserHelicoptere.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZonePoserHelicoptere.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZonePoserHelicoptere, "MIL_ZonePoserHelicoptere" )

//-----------------------------------------------------------------------------
// Name: MIL_ZonePoserHelicoptere constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZonePoserHelicoptere::MIL_ZonePoserHelicoptere( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_RealObject_ABC( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_ZonePoserHelicoptere constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZonePoserHelicoptere::MIL_ZonePoserHelicoptere()
    : MIL_RealObject_ABC()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZonePoserHelicoptere destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZonePoserHelicoptere::~MIL_ZonePoserHelicoptere()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZonePoserHelicoptere::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZonePoserHelicoptere::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}
