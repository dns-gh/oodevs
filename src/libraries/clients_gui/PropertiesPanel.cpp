// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesPanel.h"
#include "moc_PropertiesPanel.cpp"
#include "GlProxy.h"
#include "PropertyTreeView.h"
#include "PropertyModel.h"
#include "PropertyDelegate.h"
#include "PropertyDisplayer.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_gui/PropertiesGroupDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, EditorFactory_ABC& factory,
                                  PropertyDisplayer& displayer, PropertyDisplayer& comparator, const GlProxy& glProxy )
    : QScrollArea( parent )
    , controllers_     ( controllers )
    , glProxy_         ( glProxy )
    , selected_        ( 0 )
    , view_            ( new PropertyTreeView( "propertyView" ) )
    , delegate_        ( new PropertyDelegate( controllers.actions_, factory ) )
    , model_           ( new PropertyModel( displayer ) )
    , pMultiProperties_( new PropertiesGroupDictionary( controllers.controller_, comparator ) )
{
    view_->setModel( model_ );
    view_->setItemDelegate( delegate_ );
    setWidget( view_ );
    setWidgetResizable( true );
    connect( model_, SIGNAL( InternalItemChanged() ), this, SLOT( OnItemChanged() ) );
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
    if( selected_ != 0 )
    {
        const kernel::Entity_ABC* selected = selected_;
        selected_ = 0;
        NotifySelected( selected );
    }
    else if( !urbanObjects_.empty() )
    {
        std::vector< const kernel::UrbanObject_ABC* > urbanObjects = urbanObjects_;
        urbanObjects_.clear();
        NotifySelectionChanged( urbanObjects );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifySelected
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( !element || selected_ != element )
    {
        ClearSelection();
        if( element )
            if( gui::PropertiesDictionary* dictionary = const_cast< kernel::Entity_ABC* >( element )->Retrieve< gui::PropertiesDictionary >() )
            {
                dictionary->Display( *model_ );
                view_->Display();
                selected_ = element;
            }
    }
    setWidget( view_ );
    if( element )
        view_->setEnabled( glProxy_.ShouldEdit( *element ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyUpdated
// Created: MCO 2013-05-17
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyUpdated( const kernel::Entity_ABC& element )
{
    if( selected_ == &element )
    {
        view_->SaveState();
        model_->clear();
        if( gui::PropertiesDictionary* dictionary = const_cast< kernel::Entity_ABC& >( element ).Retrieve< gui::PropertiesDictionary >() )
        {
            dictionary->Display( *model_ );
            view_->Display();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifySelectionChanged
// Created: LGY 2013-01-03
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifySelectionChanged( const std::vector< const kernel::UrbanObject_ABC* >& elements )
{
    if( elements.size() > 1 )
    {
        ClearSelection();
        pMultiProperties_->Fill( elements );
        pMultiProperties_->Display( *model_ );
        view_->Display();
        urbanObjects_ = elements;
    }
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
void PropertiesPanel::NotifyUpdated( const DictionaryUpdated& message )
{
    if( selected_ && selected_->GetId() == message.GetEntity().GetId() )
        model_->Update( message.GetEntry() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: LGY 2012-08-22
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const DictionaryUpdated& message )
{
    if( selected_ && selected_->GetId() == message.GetEntity().GetId() )
        model_->Delete( message.GetEntry() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyCreated
// Created: LGY 2012-09-05
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyCreated( const DictionaryUpdated& message )
{
    if( selected_ && selected_->GetId() == message.GetEntity().GetId() )
    {
        if( view_->Exist( message.GetEntry() ) )
            NotifyUpdated( message );
        else if( PropertiesDictionary* dictionary = const_cast< kernel::Entity_ABC* >( selected_ )->Retrieve< PropertiesDictionary >() )
        {
            dictionary->Display( message.GetEntry(), *model_ );
            view_->Display();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::OnItemChanged
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
void PropertiesPanel::OnItemChanged()
{
    if( selected_ )
    {
        selected_->Select( controllers_.actions_ );

        kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
        list.push_back( selected_ );
        selected_->MultipleSelect( controllers_.actions_, list );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::ClearSelection
// Created: LGY 2013-01-29
// -----------------------------------------------------------------------------
void PropertiesPanel::ClearSelection()
{
    view_->SaveState();
    selected_ = 0;
    urbanObjects_.clear();
    model_->clear();
}
