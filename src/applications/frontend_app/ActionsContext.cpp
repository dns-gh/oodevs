// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionsContext.h"
#include <Qt3Support/q3listbox.h>

// -----------------------------------------------------------------------------
// Name: ActionsContext constructor
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
ActionsContext::ActionsContext()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsContext destructor
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
ActionsContext::~ActionsContext()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsContext::Save
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void ActionsContext::Save( const std::string& name, const QString& value )
{
    values_[ name ] = value.isEmpty() ? "" : value.ascii();
}

// -----------------------------------------------------------------------------
// Name: ActionsContext::GetValue
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
std::string ActionsContext::GetValue( const std::string& name ) const
{
    T_Values::const_iterator it = values_.find( name );
    if( it != values_.end() )
        return it->second;
    return "";
}

// -----------------------------------------------------------------------------
// Name: ActionsContext::Load
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void ActionsContext::Load( const std::string& name, Q3ListBox* widget ) const
{
    if( widget )
        widget->setSelected( widget->findItem( GetValue( name ).c_str() ), true );
}

// -----------------------------------------------------------------------------
// Name: ActionsContext::Save
// Created: SBO 2008-04-16
// -----------------------------------------------------------------------------
void ActionsContext::Save( const std::string& name, const Q3ListBox* widget )
{
    if( widget && widget->selectedItem() )
         Save( name, widget->selectedItem()->text().ascii() );
}
