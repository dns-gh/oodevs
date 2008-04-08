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
#include "clients_kernel/EditorFactory_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/PropertiesWidget.h"
#include "clients_gui/TableItemDisplayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory, gui::TableItemDisplayer& displayer )
    : QScrollView( parent )
    , controllers_( controllers )
    , selected_( controllers )
    , displayer_( displayer )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );

    QVBox* box = new QVBox( viewport() );
    addChild( box );
    box->setMargin( 5 );
    box->setSpacing( 5 );

    table_ = new gui::PropertiesWidget( controllers_.controller_, box, tr( "Properties" ), factory, displayer_ );
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
        table_->Clear();
        selected_ = element;
        if( selected_ )
            if( kernel::PropertiesDictionary* dico = const_cast< kernel::Entity_ABC* >( element )->Retrieve< kernel::PropertiesDictionary >() )
            {
                dico->Display( *table_ );
                return;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const kernel::Entity_ABC& element )
{
    if( selected_ = &element )
        NotifySelected( 0 );
}
