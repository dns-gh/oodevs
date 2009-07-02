// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AgentTypePionREFUGIE.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 16:31 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionREFUGIE.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentTypePionREFUGIE.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionREFUGIE::MIL_AgentTypePionREFUGIE( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionREFUGIE::~MIL_AgentTypePionREFUGIE()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePionREFUGIE::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePionREFUGIE( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_AgentTypePionREFUGIE::IsRefugee() const
{
    return true;
}
