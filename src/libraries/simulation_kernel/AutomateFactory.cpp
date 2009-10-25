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

#include "Tools/MIL_IDManager.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( AutomateFactory, "AutomateFactory" )

template< typename Archive >
void save_construct_data( Archive& archive, const AutomateFactory* factory, const unsigned int /*version*/ )
{
    const MIL_IDManager* const idManager = &factory->idManager_;
    archive << idManager;
}
template< typename Archive >
void load_construct_data( Archive& archive, AutomateFactory* factory, const unsigned int /*version*/ )
{
    MIL_IDManager* idManager;
    archive >> idManager;
    ::new( factory )AutomateFactory( *idManager );
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory constructor
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
AutomateFactory::AutomateFactory( MIL_IDManager& idManager )
    : idManager_( idManager ) 
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
{//@TODO MMgd Method's pattern
    uint id;
    std::string strType;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", strType );

    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( strType );

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, xis );
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

    MIL_Automate& automate = pType->InstanciateAutomate( id, parent, xis );
    automate.ReadOverloading( xis );

    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );

    return automate;
}
