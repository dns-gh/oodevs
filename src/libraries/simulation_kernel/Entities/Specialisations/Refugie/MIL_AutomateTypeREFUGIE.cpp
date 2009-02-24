// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Refugie/MIL_AutomateTypeREFUGIE.cpp $
// $Author: Nld $
// $Modtime: 10/03/05 17:30 $
// $Revision: 1 $
// $Workfile: MIL_AutomateTypeREFUGIE.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateTypeREFUGIE.h"
#include "Decision/DEC_Model_ABC.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeREFUGIE::MIL_AutomateTypeREFUGIE( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeREFUGIE destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeREFUGIE::~MIL_AutomateTypeREFUGIE()
{

}

