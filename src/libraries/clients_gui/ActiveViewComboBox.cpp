// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ActiveViewComboBox.h"
#include "GLView_ABC.h"
#include "GLMainProxy.h"
#include "SignalAdapter.h"

using namespace gui;

Q_DECLARE_METATYPE( GLView_ABC::T_View )

// -----------------------------------------------------------------------------
// Name: ActiveViewComboBox constructor
// Created: ABR 2014-07-23
// -----------------------------------------------------------------------------
ActiveViewComboBox::ActiveViewComboBox( GLMainProxy& mainProxy,
                                        const QString& objectName,
                                        QWidget* parent /*= 0*/ )
    : QComboBox( parent, objectName )
{
    mainProxy.AddCreationObserver( this, [&]( const GLView_ABC::T_View& view ) {
        addItem( view->GetName(), QVariant::fromValue( view ) );
        setEnabled( count() > 1 );
    } );
    mainProxy.AddDeletionObserver( this, [ &]( const GLView_ABC::T_View& view ) {
        removeItem( findText( view->GetName() ) );
        setEnabled( count() > 1 );
    } );
    mainProxy.AddActiveChangeObserver( this, [ &]( const GLView_ABC::T_View& view ) {
        setCurrentIndex( findText( view->GetName() ) );
    } );
    gui::connect( this, SIGNAL( activated( int ) ), [&]{
        mainProxy.SetActiveView( *itemData( currentIndex() ).value< GLView_ABC::T_View >() );
    } );
}

// -----------------------------------------------------------------------------
// Name: ActiveViewComboBox destructor
// Created: ABR 2014-07-23
// -----------------------------------------------------------------------------
ActiveViewComboBox::~ActiveViewComboBox()
{
    // NOTHING
}
