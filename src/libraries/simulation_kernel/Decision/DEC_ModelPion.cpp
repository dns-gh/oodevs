// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/DEC_ModelPion.cpp $
// $Author: Nld $
// $Modtime: 4/03/05 14:26 $
// $Revision: 5 $
// $Workfile: DEC_ModelPion.cpp $
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_ModelPion.h"

#include "Entities/Orders/MIL_PionMissionType.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_ModelPion::DEC_ModelPion( const DEC_Workspace& decWorkspace, const std::string& strName, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath )
    : DEC_Model_ABC< MIL_PionMissionType >( decWorkspace, strName, xis, bNeedParsing, bUseOnlyArchive, strBinaryPath, "units" )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelPion destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
DEC_ModelPion::~DEC_ModelPion()
{

}


