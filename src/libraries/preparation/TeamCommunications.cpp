// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamCommunications.h"

// -----------------------------------------------------------------------------
// Name: TeamCommunications constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
TeamCommunications::TeamCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : CommunicationHierarchies( controller, holder, superior )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamCommunications destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
TeamCommunications::~TeamCommunications()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamCommunications::DoSerialize
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void TeamCommunications::DoSerialize( xml::xostream& xos ) const
{
    // $$$$ SBO 2006-09-25: todo
}
