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

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC constructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
InfoPanel_ABC::InfoPanel_ABC( QWidget* pParent )
    : QScrollView( pParent )
    , pBox_( new QVBox( viewport() ) )
{
    setHScrollBarMode( QScrollView::AlwaysOff );
    pBox_->setMargin( 5 );
    pBox_->setSpacing( 5 );
    addChild( pBox_ );
    setResizePolicy( AutoOneFit );
    setFrameStyle( QFrame::Box | QFrame::Sunken );
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
