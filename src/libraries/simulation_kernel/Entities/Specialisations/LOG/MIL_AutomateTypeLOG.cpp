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
#include "MIL_AutomateTypeLOG.h"
#include "MIL_AutomateLOG.h"
#include "Entities/Automates/MIL_Automate.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::MIL_AutomateTypeLOG( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeLOG::~MIL_AutomateTypeLOG()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::IsLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateTypeLOG::IsLogistic() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID,
                                                        MissionController_ABC& controller,
                                                        MIL_Entity_ABC& parent,
                                                        xml::xistream& xis,
                                                        unsigned int gcPause,
                                                        unsigned int gcMult,
                                                        sword::DEC_Logger* logger ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, xis, gcPause, gcMult, logger );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::InstanciateAutomate
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateTypeLOG::InstanciateAutomate( unsigned int nID,
                                                        MissionController_ABC& controller,
                                                        MIL_Entity_ABC& parent,
                                                        unsigned int knowledgeGroup,
                                                        const std::string& name,
                                                        unsigned int gcPause,
                                                        unsigned int gcMult,
                                                        sword::DEC_Logger* logger,
                                                        unsigned int context,
                                                        const MIL_DictionaryExtensions& extensions ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, knowledgeGroup, name, gcPause, gcMult, logger, context, extensions );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeLOG::Create
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateTypeLOG::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeLOG( strName, xis );
}


