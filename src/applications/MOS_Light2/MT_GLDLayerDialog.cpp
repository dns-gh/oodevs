// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDLayerDialog.cpp $
// $Author: Ape $
// $Modtime: 29/06/04 11:25 $
// $Revision: 1 $
// $Workfile: MT_GLDLayerDialog.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLDLayerDialog.h"
#include "moc_MT_GLDLayerDialog.cpp"

#include "MT_GLDLayer.h"
#include "MT_Slider.h"

#include <qlineedit.h>

#ifndef MT_USE_INLINE
#   include "MT_GLDLayerDialog.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDLayerDialog constructor
/** @param  pParent 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDLayerDialog::MT_GLDLayerDialog( QWidget* pParent )
    : QDialog( pParent )
    , pLayer_( 0 )
{
    this->setCaption( tr( "Propriétés du calque" ) );

    QLabel* pNameLabel = new QLabel( tr( "Nom:" ), this );
    pNameEdit_ = new QLineEdit( this );

    QLabel* pOpacityLabel = new QLabel( tr( "Opacité:" ), this );
    pOpacitySlider_ = new MT_Slider( 1, 100, 1, 1, " %", MT_Slider::eBottomRight, Qt::Horizontal, this );

    pOkButton_     = new QPushButton( "OK", this );
    pApplyButton_  = new QPushButton( "Appliquer", this );
    pCancelButton_ = new QPushButton( "Annuler", this );

    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setSpacing( 5 );
    pLayout->setMargin( 5 );

    QHBoxLayout* pSubLayout1 = new QHBoxLayout( pLayout );
    pSubLayout1->addWidget( pNameLabel, 1 );
    pSubLayout1->addWidget( pNameEdit_, 1 );

    QHBoxLayout* pSubLayout2 = new QHBoxLayout( pLayout );
    pSubLayout2->addWidget( pOpacityLabel, 1 );
    pSubLayout2->addWidget( pOpacitySlider_, 0 );

    pLayout->addSpacing( 15 );

    QHBoxLayout* pSubLayout3 = new QHBoxLayout( pLayout );
    pSubLayout3->addSpacing( 20 );
    pSubLayout3->addWidget( pOkButton_ );
    pSubLayout3->addWidget( pApplyButton_ );
    pSubLayout3->addWidget( pCancelButton_ );

    connect( pOkButton_,     SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pOkButton_,     SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( hide() ) );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayerDialog destructor
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDLayerDialog::~MT_GLDLayerDialog()
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayerDialog::SetLayer
/** @param  pLayer 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDLayerDialog::SetLayer( MT_GLDLayer* pLayer )
{
    if( pLayer == 0 )
    {
        this->hide();
        return;
    }

    pLayer_ = pLayer;
    pNameEdit_->setText( pLayer->GetName() );
    pOpacitySlider_->SetValue( (int)(pLayer->GetOpacity() * 100) );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDLayerDialog::OnApply
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDLayerDialog::OnApply()
{
    assert( pLayer_ != 0 );
    pLayer_->SetName( pNameEdit_->text() );
    pLayer_->SetOpacity( pOpacitySlider_->GetValue() / 100.0f );
    emit LayerEdited( *pLayer_ );
}
