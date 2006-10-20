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
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );

    addChild( pBox_ );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );

    table_ = new gui::PropertiesWidget( pBox_, tools::translate( "Properties", "Properties" ) );

    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: Properties destructor
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Properties::~Properties()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Properties::NotifySelected
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Properties::NotifySelected( const Entity_ABC* element )
{
    if( element )
    {
        PropertiesDictionary* dico = const_cast< Entity_ABC* >( element )->Retrieve< PropertiesDictionary >();
        if( dico )
            dico->Display( *table_ );
    }
}

// -----------------------------------------------------------------------------
// Name: Properties::insertChild
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void Properties::insertChild( QObject* pObj )
{
    pBox_->insertChild( pObj );
}

// -----------------------------------------------------------------------------
// Name: Properties::layout
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
QLayout* Properties::layout()
{
    if( !pBox_ )
        return 0;
    return pBox_->layout();
}
