//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridgeContinuous.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:25 $
// $Revision: 11 $
// $Workfile: MIL_FloatingBridgeContinuous.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_FloatingBridgeContinuous.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_FloatingBridgeContinuous, "MIL_FloatingBridgeContinuous" )

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeContinuous constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridgeContinuous::MIL_FloatingBridgeContinuous( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_FloatingBridge_ABC( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeContinuous constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_FloatingBridgeContinuous::MIL_FloatingBridgeContinuous()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeContinuous destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridgeContinuous::~MIL_FloatingBridgeContinuous()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeContinuous::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridgeContinuous::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    LinkToPathFinder();
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeContinuous::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridgeContinuous::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

