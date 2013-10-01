// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Alat/MIL_AgentTypePionALAT.cpp $
// $Author: Jvt $
// $Modtime: 24/05/05 10:17 $
// $Revision: 19 $
// $Workfile: MIL_AgentTypePionALAT.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionALAT.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"
#include "Decision/DEC_PerceptionFunctions.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::MIL_AgentTypePionALAT( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentTypePion( strName, strType, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionALAT destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionALAT::~MIL_AgentTypePionALAT()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionINF::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionALAT::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePionALAT( strName, strType, xis );
}
