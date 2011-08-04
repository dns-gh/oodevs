//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ProgressBar.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 18:22 $
// $Revision: 4 $
// $Workfile: ADN_ProgressBar.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ProgressBar.h"

//-----------------------------------------------------------------------------
// Name: ADN_ProgressBar constructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_ProgressBar::ADN_ProgressBar( QWidget* pParent, const char* szName, Qt::WFlags f )
    : Q3HBox( pParent, szName, f )
    , ADN_ProgressIndicator_ABC()
{
    label_ = new QLabel( this );
    label_->setAlignment( Qt::AlignLeft );
    bar_ = new Q3ProgressBar( this );
    setStretchFactor( label_, 2 );
    setStretchFactor( bar_, 1 );
}


//-----------------------------------------------------------------------------
// Name: ADN_ProgressBar destructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_ProgressBar::~ADN_ProgressBar()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::SetNbrOfSteps
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::SetNbrOfSteps( int n )
{
    bar_->setTotalSteps( n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Increment( int n )
{
    bar_->setProgress( bar_->progress() + n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Increment( const char* szText, int n )
{
    label_->setText( szText );
    Increment( n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Reset
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Reset( const char* szMsg )
{
    label_->setText( szMsg );
    bar_->reset();
}
