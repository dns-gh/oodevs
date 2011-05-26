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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

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
AutomatCommunications::AutomatCommunications( xml::xistream& xis, kernel::Controller& controller, kernel::Entity_ABC& holder, const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groups )
    : EntityCommunications( controller, holder, ReadKnowledgeGroup( xis, groups ) )
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
// Name: AutomatCommunications::ReadKnowledgeGroup
// Created: AGE 2006-10-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* AutomatCommunications::ReadKnowledgeGroup( xml::xistream& xis, const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groups )
{
    int group;
    xis >> xml::attribute( "knowledge-group", group );
    return groups.Find( group );
}

// -----------------------------------------------------------------------------
// Name: AutomatCommunications::SerializeAttributes
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AutomatCommunications::SerializeAttributes( xml::xostream& xos ) const
{
    if( ! GetSuperior() ) // $$$$ SBO 2006-10-09: check that superior is a gtia
        throw std::runtime_error( QString( "Knowledge group not defined for automat '%1'." ).arg( GetEntity().GetId() ).ascii() );
    xos << xml::attribute( "knowledge-group", long( GetSuperior()->GetId() ) );
}
