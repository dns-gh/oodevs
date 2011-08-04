// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_SplashScreen.cpp $
// $Author: Ape $
// $Modtime: 21/03/05 18:37 $
// $Revision: 1 $
// $Workfile: ADN_SplashScreen.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SplashScreen.h"

// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen constructor
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_SplashScreen::ADN_SplashScreen( const QPixmap& pixmap, Qt::WFlags f )
: QSplashScreen            ( pixmap, f )
, ADN_ProgressIndicator_ABC()
, textColor_               ( Qt::black )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen constructor
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_SplashScreen::ADN_SplashScreen()
: QSplashScreen            ()
, ADN_ProgressIndicator_ABC()
, textColor_               ( Qt::black )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen destructor
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_SplashScreen::~ADN_SplashScreen()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::SetNbrOfSteps
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_SplashScreen::SetNbrOfSteps( int /*n*/ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::SetTextColor
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_SplashScreen::SetTextColor( const QColor& color )
{
    textColor_ = color;
}


// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_SplashScreen::Increment( int /*n*/ )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_SplashScreen::Increment( const char* szText, int /*n*/ )
{
    if( IsValid() )
        this->message( szText, Qt::AlignBottom | Qt::AlignHCenter, textColor_ );
    qApp->processEvents();
}


// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::Reset
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_SplashScreen::Reset( const char* szMsg )
{
    if( IsValid() )
        this->message( szMsg, Qt::AlignBottom | Qt::AlignHCenter, textColor_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_SplashScreen::IsValid
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
bool ADN_SplashScreen::IsValid() const
{
    return !pixmap().isNull();
}
