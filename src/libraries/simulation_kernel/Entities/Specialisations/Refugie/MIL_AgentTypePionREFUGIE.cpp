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
#include "xeumeuleu/xml.h"



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionREFUGIE::MIL_AgentTypePionREFUGIE( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionREFUGIE destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionREFUGIE::~MIL_AgentTypePionREFUGIE()
{

}
