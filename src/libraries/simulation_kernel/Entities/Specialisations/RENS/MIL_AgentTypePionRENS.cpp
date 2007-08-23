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
#include "Decision/Functions/DEC_PerceptionFunctions.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionRENS constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionRENS::MIL_AgentTypePionRENS( const std::string& strName, xml::xistream& xis )
    : MIL_AgentTypePion( strName, xis )
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRecordMode , "DEC_Perception_ActiverModeEnregistrement"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRecordMode, "DEC_Perception_DesactiverModeEnregistrement" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionRENS destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionRENS::~MIL_AgentTypePionRENS()
{

}
