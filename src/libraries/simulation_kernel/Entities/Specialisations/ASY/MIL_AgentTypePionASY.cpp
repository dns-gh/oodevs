// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/ASY/MIL_AgentTypePionASY.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 16:31 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionASY.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionASY.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionASY constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionASY::MIL_AgentTypePionASY( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionASY destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionASY::~MIL_AgentTypePionASY()
{

}
