// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Nbc/MIL_AgentTypePionNBC.cpp $
// $Author: Nld $
// $Modtime: 25/03/05 16:40 $
// $Revision: 5 $
// $Workfile: MIL_AgentTypePionNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePionNBC.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_AgentFunctions.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionNBC::MIL_AgentTypePionNBC( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePion( strName, archive )
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::DecontaminateZone, "DEC_DecontaminerZone"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions          ::SelfDecontaminate, "DEC_Agent_SeDecontaminer" );

}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionNBC destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionNBC::~MIL_AgentTypePionNBC()
{

}
