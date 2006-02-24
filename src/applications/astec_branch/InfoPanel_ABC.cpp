// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-05 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel_ABC.cpp $
// $Author: Age $
// $Modtime: 5/04/05 19:08 $
// $Revision: 1 $
// $Workfile: InfoPanel_ABC.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "InfoPanel_ABC.h"
#include "InfoPanel.h"

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC constructor
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
InfoPanel_ABC::InfoPanel_ABC( InfoPanel* parent, const QString& name )
    : QScrollView( parent )
    , parent_( parent )
    , name_( name )
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
// Name: InfoPanel_ABC destructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
InfoPanel_ABC::~InfoPanel_ABC()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::Show
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel_ABC::Show()
{
    parent_->Add( this, name_ );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::Hide
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel_ABC::Hide()
{
    parent_->Remove( this );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::insertChild
// Created: AGE 2005-09-21
// -----------------------------------------------------------------------------
void InfoPanel_ABC::insertChild( QObject* pObj )
{
    pBox_->insertChild( pObj );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::layout
// Created: SBO 2005-09-23
// -----------------------------------------------------------------------------
QLayout* InfoPanel_ABC::layout()
{
    if( !pBox_ )
        return 0;
    return pBox_->layout();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::sizeHint
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
QSize InfoPanel_ABC::sizeHint() const
{
    return QSize( 640, 1280 );
}
