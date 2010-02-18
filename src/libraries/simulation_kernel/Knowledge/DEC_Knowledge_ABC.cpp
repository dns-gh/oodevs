// *****************************************************************************
//
// $Created: NLD 2004-03-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ABC.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 12:13 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ABC.h"
#include "MIL_AgentServer.h"
#include "MIL_Singletons.h"
#include "MIL_Time_ABC.h"
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ABC )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ABC constructor
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
DEC_Knowledge_ABC::DEC_Knowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ABC destructor
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
DEC_Knowledge_ABC::~DEC_Knowledge_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ABC::GetCurrentTimeStep
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
uint DEC_Knowledge_ABC::GetCurrentTimeStep() const
{
    return MIL_Singletons::GetTime().GetCurrentTick();
}
