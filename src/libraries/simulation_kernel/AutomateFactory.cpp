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
MIL_Automate* AutomateFactory::Create( const MIL_AutomateType& type, MIL_Automate& parent, xml::xistream& xis )
{
    uint        id;
    xis >> xml::attribute( "id", id );

    MIL_Automate* pAutomate = &type.InstanciateAutomate( id, parent, xis );
    pAutomate->ReadOverloading( xis );
    return pAutomate;
}

// -----------------------------------------------------------------------------
// Name: AutomateFactory::Create
// Created: MGD 2009-08-17
// -----------------------------------------------------------------------------
MIL_Automate* AutomateFactory::Create( const MIL_AutomateType& type, MIL_Formation& parent, xml::xistream& xis )
{
    uint        id;
    xis >> xml::attribute( "id", id );

    MIL_Automate* pAutomate = &type.InstanciateAutomate( id, parent, xis );
    pAutomate->ReadOverloading( xis );
    return pAutomate;
}
