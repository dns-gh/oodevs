// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "OrbatAttributesDialog.h"
#include "moc_OrbatAttributesDialog.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "gaming/Attributes.h"

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog constructor
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
OrbatAttributesDialog::OrbatAttributesDialog( QWidget* pParent, kernel::Controllers& controllers )
    : QDialog     ( pParent, 0, TRUE )
    , controllers_( controllers )
    , extensions_ ( 0 )
    , layout_     ( 0 )
{
    setCaption( tr( "Extensions" ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog destructor
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
OrbatAttributesDialog::~OrbatAttributesDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::NotifyContextMenu
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    extensions_ = 0;
    const Attributes* attributes = static_cast< const Attributes* >( entity.Retrieve< kernel::Attributes_ABC >() );
    if( attributes )
        CreateSubMenu( menu, attributes->GetExtensions() );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::NotifyContextMenu
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    extensions_ = 0;
    const kernel::DictionaryExtensions* extensions = entity.Retrieve< kernel::DictionaryExtensions >();
    if( extensions )
        CreateSubMenu(menu, extensions->GetExtensions() );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::NotifyContextMenu
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    extensions_ = 0;
    const kernel::DictionaryExtensions* extensions = entity.Retrieve< kernel::DictionaryExtensions >();
    if( extensions )
        CreateSubMenu( menu, extensions->GetExtensions() );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::NotifyContextMenu
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    extensions_ = 0;
    const kernel::DictionaryExtensions* extensions = entity.Retrieve< kernel::DictionaryExtensions >();
    if( extensions )
        CreateSubMenu( menu, extensions->GetExtensions() );
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::CreateSubMenu
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::CreateSubMenu( kernel::ContextMenu& menu, const std::map< std::string, std::string >& extensions )
{
    if( extensions.size() )
    {
        extensions_ = &extensions;
        menu.InsertItem( "Helpers", tr( "Extensions" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatAttributesDialog::Show
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void OrbatAttributesDialog::Show()
{
    if( extensions_ )
    {
        for( std::vector< QLabel* >::const_iterator it = labels_.begin(); it != labels_.end(); ++it )
            delete *it;
        labels_.clear();
        delete layout_;
        layout_ = new QGridLayout( this, extensions_->size(), 2, 5, 5 );
        unsigned int line = 0;
        for( std::map< std::string, std::string >::const_iterator it = extensions_->begin(); it != extensions_->end(); ++it )
        {
            QLabel* key = new QLabel( it->first.c_str(), this );
            QLabel* value = new QLabel( it->second.c_str(), this );
            layout_->addWidget( key, line, 0 );
            layout_->addWidget( value, line, 1 );
            labels_.push_back( key );
            labels_.push_back( value );
            ++line;
        }
        show();
    }
}
