//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridgeDiscontinuous.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 15:25 $
// $Revision: 11 $
// $Workfile: MIL_FloatingBridgeDiscontinuous.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_FloatingBridgeDiscontinuous.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_FloatingBridgeDiscontinuous, "MIL_FloatingBridgeDiscontinuous" )

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeDiscontinuous constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridgeDiscontinuous::MIL_FloatingBridgeDiscontinuous( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_FloatingBridge_ABC( type, nID, army )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeDiscontinuous constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_FloatingBridgeDiscontinuous::MIL_FloatingBridgeDiscontinuous()
    : MIL_FloatingBridge_ABC()
{
}

//-----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeDiscontinuous destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_FloatingBridgeDiscontinuous::~MIL_FloatingBridgeDiscontinuous()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeDiscontinuous::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridgeDiscontinuous::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_RealObject_ABC >( *this );
    LinkToPathFinder();
}

// -----------------------------------------------------------------------------
// Name: MIL_FloatingBridgeDiscontinuous::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_FloatingBridgeDiscontinuous::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

