// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PropertiesPanel */

#include "clients_gui_pch.h"
#include "PropertiesPanel.h"
#include "PropertyTreeView.h"
#include "PropertyModel.h"
#include "PropertyDelegate.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "PropertyDisplayer.h"
#include "GlProxy.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory, PropertyDisplayer& displayer, const GlProxy& glProxy )
    : QScrollArea( parent )
    , controllers_( controllers )
    , glProxy_    ( glProxy )
    , selected_   ( 0 )
    , view_       ( new PropertyTreeView() )
    , delegate_   ( new PropertyDelegate( factory ) )
    , model_      ( new PropertyModel( displayer ) )
{
    view_->setModel( model_ );
    view_->setItemDelegate( delegate_ );
    setWidget( view_ );
    setWidgetResizable( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel destructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::~PropertiesPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::showEvent
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifySelected
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( !element || selected_ != element )
    {
        view_->SaveState();
        model_->clear();
        selected_ = element;

        if( element )
            if( kernel::PropertiesDictionary* dico = const_cast< kernel::Entity_ABC* >( element )->Retrieve< kernel::PropertiesDictionary >() )
            {
                dico->Display( *model_ );
                view_->Display();
            }
    }
    setWidget( view_ );
    if( element )
        view_->setEnabled( glProxy_.ShouldEdit( *element ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const kernel::Entity_ABC& element )
{
    if( selected_ && selected_->GetId() == element.GetId() )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyUpdated
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyUpdated( const kernel::DictionaryUpdated& message )
{
    if( selected_ && selected_->GetId() == message.GetEntity().GetId() )
        model_->Update( message.GetEntry() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const kernel::DictionaryUpdated& message )
{
    if( selected_ && selected_->GetId() == message.GetEntity().GetId() )
        model_->Delete( message.GetEntry() );

}
