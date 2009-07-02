// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Milice/MIL_AgentTypePionMILICE.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 16:31 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionMILICE.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionMILICE.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionMILICE::MIL_AgentTypePionMILICE( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionMILICE::~MIL_AgentTypePionMILICE()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionMILICE::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionMILICE( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionMILICE::IsMilitia
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_AgentTypePionMILICE::IsMilitia() const
{
    return true;
}
