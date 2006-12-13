// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/DEC_ModelPopulation.cpp $
// $Author: Nld $
// $Modtime: 4/03/05 16:18 $
// $Revision: 5 $
// $Workfile: DEC_ModelPopulation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_ModelPopulation.h"

#include "Entities/Orders/MIL_PopulationMissionType.h"

// -----------------------------------------------------------------------------
// Name: DEC_ModelPopulation constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_ModelPopulation::DEC_ModelPopulation( const DEC_Workspace& decWorkspace, const std::string& strName, MIL_InputArchive& input, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strArchivePath, const std::string& strSourcePath )
    : DEC_Model_ABC< MIL_PopulationMissionType >( decWorkspace, strName, input, bNeedParsing, bUseOnlyArchive, strArchivePath, strSourcePath )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPopulation destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
DEC_ModelPopulation::~DEC_ModelPopulation()
{

}
