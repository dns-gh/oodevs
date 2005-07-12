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
#include "ADN_Pch.h"
#include "ADN_ProgressBar.h"

#include <qlayout.h>

//-----------------------------------------------------------------------------
// Name: ADN_ProgressBar constructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_ProgressBar::ADN_ProgressBar( QWidget* pParent, const char* szName, WFlags f )
: QProgressBar             ( pParent, szName, f )
, ADN_ProgressIndicator_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_ProgressBar destructor
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
ADN_ProgressBar::~ADN_ProgressBar()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::SetNbrOfSteps
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::SetNbrOfSteps( int n )
{
    this->setTotalSteps( n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Increment( int n )
{
    this->setProgress( progress() + n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Increment
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Increment( const char* /*szText*/, int n )
{
    this->Increment( n );
}


// -----------------------------------------------------------------------------
// Name: ADN_ProgressBar::Reset
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
void ADN_ProgressBar::Reset( const char* /*szMsg*/ )
{
    this->reset();
}
