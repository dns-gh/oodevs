// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EntityListParameter.h"
#include "moc_EntityListParameter.cpp"

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::EntityListParameterBase
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityListParameterBase::EntityListParameterBase( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::~EntityListParameterBase
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityListParameterBase::~EntityListParameterBase()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::BuildInterface
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void EntityListParameterBase::BuildInterface( QWidget* parent )
{
    EnableCreation( false );
    ListParameter::BuildInterface( parent );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameterBase::MakeMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void EntityListParameterBase::MakeMenu( const QString& label, kernel::ContextMenu& menu )
{
    menu.InsertItem( "Parameter", label, this, SLOT( MenuItemValidated() ) );
}
