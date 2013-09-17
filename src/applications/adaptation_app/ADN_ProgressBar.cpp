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
ADN_ProgressBar::ADN_ProgressBar( QWidget* parent )
    : QWidget( parent )
{
    QHBoxLayout* layout = new QHBoxLayout( this );
    label_ = new QLabel();
    bar_ = new QProgressBar();
    layout->addWidget( label_, 2, Qt::AlignLeft );
    layout->addWidget( bar_, 1 );
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
// Name: ADN_ProgressBar::SetMaximum
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::SetMaximum( int n )
{
    bar_->setMaximum( n );
}

// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Increment( const QString& text )
{
    label_->setText( text );
    bar_->setValue( bar_->value() + 1 );
    qApp->processEvents();
}

// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Reset
// Created: ABR 2013-09-17
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Reset()
{
    bar_->reset();
}
