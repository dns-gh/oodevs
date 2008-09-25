// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/DEC_ModelAutomate.cpp $
// $Author: Nld $
// $Modtime: 4/03/05 14:26 $
// $Revision: 5 $
// $Workfile: DEC_ModelAutomate.cpp $
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "DEC_ModelAutomate.h"

#include "Entities/Orders/MIL_AutomateMissionType.h"
#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate constructor
// Created: NLD 2003-11-24
// -----------------------------------------------------------------------------
DEC_ModelAutomate::DEC_ModelAutomate( const DEC_Workspace& decWorkspace, const std::string& strName, xml::xistream& xis, bool bNeedParsing, bool bUseOnlyArchive, const std::string& strBinaryPath )
    : DEC_Model_ABC< MIL_AutomateMissionType >( decWorkspace, strName, xis, bNeedParsing, bUseOnlyArchive, strBinaryPath, "automats" )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_ModelAutomate destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
DEC_ModelAutomate::~DEC_ModelAutomate()
{

}
