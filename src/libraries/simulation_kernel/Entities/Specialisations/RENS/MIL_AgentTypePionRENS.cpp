// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Rens/MIL_AgentTypePionRENS.cpp $
// $Author: Nld $
// $Modtime: 25/03/05 16:59 $
// $Revision: 7 $
// $Workfile: MIL_AgentTypePionRENS.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionRENS.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"
#include "Decision/DEC_PerceptionFunctions.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionRENS constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionRENS::MIL_AgentTypePionRENS( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionRENS destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionRENS::~MIL_AgentTypePionRENS()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionRENS::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionRENS::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionRENS( strName, strType, xis );
}
