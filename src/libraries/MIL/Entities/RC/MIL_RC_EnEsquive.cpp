// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC_EnEsquive.cpp $
// $Author: Nld $
// $Modtime: 9/03/05 16:20 $
// $Revision: 5 $
// $Workfile: MIL_RC_EnEsquive.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RC_EnEsquive.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnEsquive constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnEsquive::MIL_RC_EnEsquive( E_RC nRC )
    : MIL_RC( nRC )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_RC destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_RC_EnEsquive::~MIL_RC_EnEsquive()
{
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnEsquive destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnEsquive::Send( const MIL_AgentPion& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_RC_EnEsquive destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_RC_EnEsquive::Send( const MIL_Automate& /*sender*/, E_RcType /*nType*/, DIA_Parameters& /*diaParams*/ ) const
{
    // NOTHING
}

