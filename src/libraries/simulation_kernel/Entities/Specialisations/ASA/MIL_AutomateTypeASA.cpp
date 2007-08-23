// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Asa/MIL_AutomateTypeASA.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 11:44 $
// $Revision: 4 $
// $Workfile: MIL_AutomateTypeASA.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateTypeASA.h"

#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/DEC_Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeASA::MIL_AutomateTypeASA( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePosDeploiementASAOmni,         "DEC_Geometrie_PosDeploiementASAOmni"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePosDeploiementASANasse,        "DEC_Geometrie_PosDeploiementMistralNasse" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau, "DEC_Geometrie_PosDeploiementDoubleRideau" );    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeASA::~MIL_AutomateTypeASA()
{

}
