// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Circulation/MIL_AgentTypePionCIRCULATION.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 11:16 $
// $Revision: 1 $
// $Workfile: MIL_AgentTypePionCIRCULATION.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_AgentTypePionCIRCULATION.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCIRCULATION::MIL_AgentTypePionCIRCULATION( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentTypePion( strName, archive )
{
    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::EquipLogisticRoute, "DEC_Circulation_EquiperItineraireLogistique"      );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePionCIRCULATION destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AgentTypePionCIRCULATION::~MIL_AgentTypePionCIRCULATION()
{

}
