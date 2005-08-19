// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_EnPause.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 15:22 $
// $Revision: 5 $
// $Workfile: MIL_RC_EnPause.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_RC_EnPause.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnPause constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnPause::MIL_RC_EnPause( E_RC nRC )
    : MIL_RC( nRC )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnPause::~MIL_RC_EnPause()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnPause destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnPause::Send( const MIL_AgentPion& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnPause destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnPause::Send( const MIL_Automate& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    // NOTHING
}

