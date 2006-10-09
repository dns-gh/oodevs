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
#include "clients_kernel/KnowledgeGroup_ABC.h"
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
// Name: AutomatCommunications constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
AutomatCommunications::AutomatCommunications( xml::xistream& xis, kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groups )
    : EntityCommunications( controller, holder, 0 )
{
    int group;
    xis >> attribute( "knowledge-group", group );
    SetSuperior( &groups.Get( group ) );
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
