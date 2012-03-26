// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateTypeLOG.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 17:59 $
// $Revision: 4 $
// $Workfile: MIL_AutomateTypeLOG.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateTypeBaseLOG.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeBaseLOG constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeBaseLOG::MIL_AutomateTypeBaseLOG( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateTypeLOG( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeBaseLOG destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeBaseLOG::~MIL_AutomateTypeBaseLOG()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeBaseLOG::Create
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateTypeBaseLOG::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeBaseLOG( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::IsLogisticBase
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateTypeBaseLOG::IsLogisticBase() const
{
    return true;
}
