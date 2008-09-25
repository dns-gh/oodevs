// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Asa/MIL_AgentTypePionASA.cpp $
// $Author: Nld $
// $Modtime: 21/12/04 10:57 $
// $Revision: 2 $
// $Workfile: MIL_AgentTypePionASA.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionASA.h"
#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionASA constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionASA::MIL_AgentTypePionASA( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionASA destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionASA::~MIL_AgentTypePionASA()
{

}
