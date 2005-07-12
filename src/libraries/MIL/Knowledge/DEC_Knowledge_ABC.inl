// *****************************************************************************
//
// $Created: NLD 2004-03-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ABC.inl $
// $Author: Jvt $
// $Modtime: 31/03/05 12:13 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_ABC.inl $
//
// *****************************************************************************

#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ABC::GetCurrentTimeStep
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_ABC::GetCurrentTimeStep() const
{
    return MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ABC::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template < typename Archive >
inline
void DEC_Knowledge_ABC::serialize( Archive&, const uint )
{
}
