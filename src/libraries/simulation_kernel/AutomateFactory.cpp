// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "AutomateFactory.h"

#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"

#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Decision/DEC_Representations.h"

#include "simulation_kernel/Decision/DEC_DataBase.h"

#include "Tools/MIL_IDManager.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( AutomateFactory, "AutomateFactory" )

// -----------------------------------------------------------------------------
// Name: AutomateFactory constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::AutomateFactory( MIL_IDManager& idManager, DEC_DataBase& database )
    : idManager_( idManager ) 
    , database_( database )
{

}

// -----------------------------------------------------------------------------
// Name: AutomateFactory destructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::~AutomateFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
MIL_Automate& AutomateFactory::Create( xml::xistream& xis, MIL_Automate& parent )
{//@TODO MGD Method's pattern
    uint id;
    std::string strType;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, xis, database_ );
    automate.ReadOverloading( xis );

    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
MIL_Automate& AutomateFactory::Create( xml::xistream& xis, MIL_Formation& parent )
{
    uint id;
    std::string strType;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, xis, database_ );
    automate.ReadOverloading( xis );

    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}
