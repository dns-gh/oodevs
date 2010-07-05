// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionTasker.h"
#include "clients_kernel/Entity_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionTasker constructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::ActionTasker( const kernel::Entity_ABC* tasker )
    : tasker_( tasker )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker destructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::~ActionTasker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::GetTasker
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ActionTasker::GetTasker() const
{
    return tasker_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::SerializeAttributes
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void ActionTasker::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "target", tasker_ ? tasker_->GetId() : 0 );
}
