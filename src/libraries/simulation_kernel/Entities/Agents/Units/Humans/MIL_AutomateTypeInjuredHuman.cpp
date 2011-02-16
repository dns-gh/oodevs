// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateTypeInjuredHuman.cpp $
// $Author: Rft $
// $Modtime: 27/04/05 17:59 $
// $Revision: 4 $
// $Workfile: MIL_AutomateTypeInjuredHuman.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateTypeInjuredHuman.h"
#include "MIL_AutomateInjuredHuman.h"
#include "Decision/DEC_Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman constructor
// Created: RFT 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeInjuredHuman::MIL_AutomateTypeInjuredHuman( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman destructor
// Created: RFT 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeInjuredHuman::~MIL_AutomateTypeInjuredHuman()
{
    // NOTHING
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman::IsInjuredHuman
// Created: RPD 2010-11-10
// -----------------------------------------------------------------------------
bool MIL_AutomateTypeInjuredHuman::IsInjuredHuman() const
{
    return true;
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman::InstanciateAutomate
// Created: RFT 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeInjuredHuman::InstanciateAutomate( unsigned int nID
                                                                , MIL_Entity_ABC& parent
                                                                , xml::xistream& xis
                                                                , unsigned int gcPause
                                                                , unsigned int gcMult ) const
{
    return *new MIL_AutomateInjuredHuman( *this, nID, parent, xis, gcPause, gcMult );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman::InstanciateAutomate
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeInjuredHuman::InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context ) const
{
    return *new MIL_AutomateInjuredHuman( *this, nID, parent, knowledgeGroup, name, gcPause, gcMult, context );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeInjuredHuman::Create
// Created: RFT 2004-12-27
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateTypeInjuredHuman::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeInjuredHuman( strName, xis );
}
