//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Piste.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_Piste.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Piste.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Piste, "MIL_Piste" )

//-----------------------------------------------------------------------------
// Name: MIL_Piste constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Piste::MIL_Piste()
    : MIL_RealObject_ABC( MIL_RealObjectType::piste_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_Piste destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Piste::~MIL_Piste()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Piste::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Piste::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );  
}
