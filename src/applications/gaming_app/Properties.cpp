// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Properties.h"
#include "clients_gui/PropertiesWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_gui/EditorFactory.h"
#include "clients_gui/TableItemDisplayer.h"
#include "gaming/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Properties constructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Properties::Properties( QWidget* parent, Controllers& controllers )
    : QScrollView( parent )
    , pBox_( new QVBox( viewport() ) )
    , controllers_( controllers )
    , factory_( *new gui::EditorFactory() )
    , tableItemDisplayer_( *new gui::TableItemDisplayer() )
    , selected_( controllers )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );

    addChild( pBox_ );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );

    table_ = new gui::PropertiesWidget( controllers_.controller_, pBox_, tools::translate( "Properties", "Properties" ), factory_, tableItemDisplayer_ );

    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: Properties destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Properties::~Properties()
{
    controllers_.Remove( *this );
    delete &factory_;
    delete &tableItemDisplayer_;
}

// -----------------------------------------------------------------------------
// Name: Properties::showEvent
// Created: AGE 2006-11-17
// -----------------------------------------------------------------------------
void Properties::showEvent( QShowEvent* )
{
    const Entity_ABC* element = selected_;
    selected_ = 0;
    NotifySelected( element );
}

// -----------------------------------------------------------------------------
// Name: Properties::NotifySelected
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Properties::NotifySelected( const Entity_ABC* element )
{
    if( isVisible() && selected_ != element )
    {
        selected_ = element;
        if( selected_ )
        {
            PropertiesDictionary* dico = const_cast< Entity_ABC* >( element )->Retrieve< PropertiesDictionary >();
            if( dico )
            {
                table_->Clear(); // $$$$ AGE 2006-10-26: 
                dico->Display( *table_ );
            }
        }
    }
}
