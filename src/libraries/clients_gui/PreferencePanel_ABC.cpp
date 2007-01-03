// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencePanel_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencePanel_ABC::PreferencePanel_ABC( QWidget* parent )
    : QScrollView( parent )
    , pBox_( new QVBox( viewport() ) )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );

    addChild( pBox_ );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );
}
    
// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencePanel_ABC::~PreferencePanel_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC::insertChild
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencePanel_ABC::insertChild( QObject* pObj )
{
    pBox_->insertChild( pObj );
}
    
// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC::layout
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
QLayout* PreferencePanel_ABC::layout()
{
    if( !pBox_ )
        return 0;
    return pBox_->layout();
}
    
// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC::sizeHint
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
QSize PreferencePanel_ABC::sizeHint() const
{
    return minimumSizeHint();
}
