// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SelectionCommand.h"
#include "gaming/CommandPublisher.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include <sstream>

// -----------------------------------------------------------------------------
// Name: SelectionCommand constructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
SelectionCommand::SelectionCommand( QObject* parent, kernel::Controllers& controllers, CommandPublisher& publisher )
    : QObject( parent )
    , controllers_( controllers )
    , publisher_( publisher )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionCommand destructor
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
SelectionCommand::~SelectionCommand()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SelectionCommand::NotifySelected
// Created: SBO 2008-07-07
// -----------------------------------------------------------------------------
void SelectionCommand::NotifySelected( const kernel::Entity_ABC* element )
{
    std::stringstream ss;
    ss << "/select";
    if( element )
        ss << " " << element->GetTypeName().toStdString() << " " << element->GetId();
    publisher_.Send( "", ss.str() );
}
