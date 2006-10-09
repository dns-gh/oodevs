// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatCommunications.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AutomatCommunications constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
AutomatCommunications::AutomatCommunications( Controller& controller, Entity_ABC& holder, Entity_ABC* superior )
    : EntityCommunications( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
AutomatCommunications::~AutomatCommunications()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCommunications::DoSerialize
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AutomatCommunications::DoSerialize( xml::xostream& xos ) const
{
    if( ! GetSuperior() ) // $$$$ SBO 2006-10-09: check that superior is a gtia
        throw std::runtime_error( QString( "Knowledge group not defined for automat '%1'." ).arg( GetEntity().GetId() ).ascii() );
    xos << attribute( "knowledge-group", long( GetSuperior()->GetId() ) );
}
