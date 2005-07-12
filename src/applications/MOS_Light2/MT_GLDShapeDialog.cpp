// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeDialog.cpp $
// $Author: Ape $
// $Modtime: 9/07/04 12:04 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeDialog.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MT_GLDShapeDialog.h"
#include "moc_MT_GLDShapeDialog.cpp"

#include "MT_GLDrawer.h"
#include "MT_Slider.h"

#include <qcolordialog.h>
#include <qlineedit.h>
#include <qslider.h>

#include "res/linepattern0.xpm"
#include "res/linepattern1.xpm"
#include "res/linepattern2.xpm"
#include "res/linepattern3.xpm"
#include "res/linepattern4.xpm"
#include "res/linepattern5.xpm"
#include "res/linepattern6.xpm"
#include "res/arrow0.xpm"
#include "res/arrow1s.xpm"
#include "res/arrow2s.xpm"
#include "res/arrow3s.xpm"
#include "res/arrow1e.xpm"
#include "res/arrow2e.xpm"
#include "res/arrow3e.xpm"

#ifndef MT_USE_INLINE
#   include "MT_GLDShapeDialog.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog constructor
/** @param  pParent 
*/
// Created: APE 2004-06-28
// -----------------------------------------------------------------------------
MT_GLDShapeDialog::MT_GLDShapeDialog( QWidget* pParent, MT_GLDrawer& glDrawer )
    : QDialog  ( pParent )
{

    this->setCaption( tr( "Propriétés" ) );

    pTextGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Texte" ), this );
    pTextGroup_->setFlat( true );

    new QLabel( tr( "Texte:    " ), pTextGroup_ );
    pTextEdit_ = new QLineEdit( pTextGroup_ );
    new QLabel( tr( "Taille:" ), pTextGroup_ );
    pFontSlider_ = new QSlider( glDrawer.GetMinFontSize() * 1024, glDrawer.GetMaxFontSize() * 1024, (glDrawer.GetMaxFontSize() - glDrawer.GetMinFontSize()) / 20.0  * 1024, glDrawer.GetMinFontSize() * 1024, Qt::Horizontal, pTextGroup_ );

    pLineGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Trait" ), this );
    pLineGroup_->setFlat( true );

    new QLabel( tr( "Couleur:    " ), pLineGroup_ );
    pLineColorButton_ = new QPushButton( pLineGroup_ );
    pLineColorButton_->setPaletteBackgroundColor( Qt::blue );
    pLineColorButton_->setMaximumSize( 20, 15 );

    new QLabel( tr("Epaisseur:"), pLineGroup_ );
    pLineWidthSlider_ = new MT_Slider( 1, 10, 1, 2, " px", MT_Slider::eBottomRight, Qt::Horizontal, pLineGroup_ );
    pLineWidthSlider_->GetLabel().setMinimumWidth( 40 );

    new QLabel( tr("Pointillés:"), pLineGroup_ );
    pLinePatternCombo_ = new MT_ValuedComboBox<MT_GLDShapeProperties::E_LinePattern>( pLineGroup_ );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern0 ), MT_GLDShapeProperties::eFull );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern1 ), MT_GLDShapeProperties::eDash1 );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern2 ), MT_GLDShapeProperties::eDash2 );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern3 ), MT_GLDShapeProperties::eDash3 );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern4 ), MT_GLDShapeProperties::eDash4 );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern5 ), MT_GLDShapeProperties::eDash5 );
    pLinePatternCombo_->AddItem( QPixmap( xpm_linepattern6 ), MT_GLDShapeProperties::eDash6 );

    pArrowGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Flèches" ), this );
    pArrowGroup_->setFlat( true );
    new QLabel( tr("Départ:    "), pArrowGroup_ );
    pArrowStartCombo_ = new MT_ValuedComboBox<MT_GLDShapeProperties::E_ArrowHead>( pArrowGroup_ );
    pArrowStartCombo_->AddItem( QPixmap( xpm_arrow0  ), MT_GLDShapeProperties::eNoArrow );
    pArrowStartCombo_->AddItem( QPixmap( xpm_arrow1s ), MT_GLDShapeProperties::ePointyArrow );
    pArrowStartCombo_->AddItem( QPixmap( xpm_arrow2s ), MT_GLDShapeProperties::eNormalArrow );
    pArrowStartCombo_->AddItem( QPixmap( xpm_arrow3s ), MT_GLDShapeProperties::eDiamondArrow );
    new QLabel( tr("Arrivée:"), pArrowGroup_ );
    pArrowEndCombo_ = new MT_ValuedComboBox<MT_GLDShapeProperties::E_ArrowHead>( pArrowGroup_ );
    pArrowEndCombo_->AddItem( QPixmap( xpm_arrow0  ), MT_GLDShapeProperties::eNoArrow );
    pArrowEndCombo_->AddItem( QPixmap( xpm_arrow1e ), MT_GLDShapeProperties::ePointyArrow );
    pArrowEndCombo_->AddItem( QPixmap( xpm_arrow2e ), MT_GLDShapeProperties::eNormalArrow );
    pArrowEndCombo_->AddItem( QPixmap( xpm_arrow3e ), MT_GLDShapeProperties::eDiamondArrow );

    pSizeGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Taille" ), this );
    pSizeGroup_->setFlat( true );
    new QLabel( tr("Taille:    "), pSizeGroup_ );
    pSizeSlider_ = new QSlider( glDrawer.GetMinFontSize() * 1024, glDrawer.GetMaxFontSize() * 1024, (glDrawer.GetMaxFontSize() - glDrawer.GetMinFontSize()) / 20.0  * 1024, glDrawer.GetMinFontSize() * 1024, Qt::Horizontal, pSizeGroup_ );

    pFillGroup_ = new QGroupBox( 2, Qt::Horizontal, tr( "Remplissage" ), this );
    pFillGroup_->setFlat( true );

    new QLabel( tr( "Couleur:    " ), pFillGroup_ );
    pFillColorButton_ = new QPushButton( pFillGroup_ );
    pFillColorButton_->setPaletteBackgroundColor( Qt::blue );
    pFillColorButton_->setMaximumSize( 20, 15 );

    new QLabel( tr("Opacité:"), pFillGroup_ );
    pFillOpacitySlider_ = new MT_Slider( 0, 100, 1, 100, " %", MT_Slider::eBottomRight, Qt::Horizontal, pFillGroup_ );
    pFillOpacitySlider_->GetLabel().setMinimumWidth( 40 );
    
    pOkButton_     = new QPushButton( "OK", this );
    pApplyButton_  = new QPushButton( "Appliquer", this );
    pCancelButton_ = new QPushButton( "Annuler", this );
    pOkButton_->setDefault( true );

    pEmptyLabel_ = new QLabel( tr( "Aucune forme sélectionnée" ), this );
    pEmptyLabel_->setAlignment( Qt::AlignCenter );
    pEmptyLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    QVBoxLayout* pLayout = new QVBoxLayout( this );
    pLayout->setSpacing( 5 );
    pLayout->setMargin( 5 );

    pLayout->addWidget( pTextGroup_ );
    pLayout->addWidget( pLineGroup_ );
    pLayout->addWidget( pArrowGroup_ );
    pLayout->addWidget( pSizeGroup_ );
    pLayout->addWidget( pFillGroup_ );
    pLayout->addWidget( pEmptyLabel_ );

    pLayout->addSpacing( 15 );
    pLayout->addStretch( 1 );

    QHBoxLayout* pSubLayout4 = new QHBoxLayout( pLayout );
    pSubLayout4->addSpacing( 20 );
    pSubLayout4->addWidget( pOkButton_ );
    pSubLayout4->addWidget( pApplyButton_ );
    pSubLayout4->addWidget( pCancelButton_ );

    pLayout->setStretchFactor( pEmptyLabel_, 2000 );
    pLayout->setStretchFactor( pSubLayout4, 0 );

    connect( pTextEdit_,   SIGNAL( textChanged( const QString& ) ), this, SLOT( OnText() ) );
    connect( pFontSlider_, SIGNAL( valueChanged( int ) ),           this, SLOT( OnFontSize() ) );
    connect( pLineColorButton_, SIGNAL( clicked() ), this, SLOT( OnLineColor() ) );
    connect( pFillColorButton_, SIGNAL( clicked() ), this, SLOT( OnFillColor() ) );
    connect( pLinePatternCombo_, SIGNAL( activated( int ) ), this, SLOT( OnLinePattern() ) );
    connect( pArrowStartCombo_, SIGNAL( activated( int ) ), this, SLOT( OnArrowStart() ) );
    connect( pArrowEndCombo_,   SIGNAL( activated( int ) ), this, SLOT( OnArrowEnd() ) );
    connect( pSizeSlider_, SIGNAL( valueChanged( int ) ), this, SLOT( OnSize() ) );
    connect( &(pLineWidthSlider_->GetSlider()), SIGNAL( valueChanged( int ) ), this, SLOT( OnLineWidth() ) );
    connect( &(pFillOpacitySlider_->GetSlider()), SIGNAL( valueChanged( int ) ), this, SLOT( OnFillOpacity() ) );
    connect( pOkButton_,     SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pOkButton_,     SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( pApplyButton_,  SIGNAL( clicked() ), this, SLOT( OnApply() ) );
    connect( pCancelButton_, SIGNAL( clicked() ), this, SLOT( hide() ) );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog destructor
// Created: APE 2004-06-28
// -----------------------------------------------------------------------------
MT_GLDShapeDialog::~MT_GLDShapeDialog()
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::SetShapes
/** @param  shapes 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::SetShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes )
{
    shapes_ = shapes;
    if( shapes.empty() )
    {
        pTextGroup_->hide();
        pLineGroup_->hide();
        pArrowGroup_->hide();
        pSizeGroup_->hide();
        pFillGroup_->hide();
        pOkButton_->setEnabled( false );
        pApplyButton_->setEnabled( false );
        pEmptyLabel_->show();
        return;
    }

    pEmptyLabel_->hide();
    MT_GLDShapeProperties properties = shapes.front()->GetProperties();
    for( MT_GLDShape_ABC::CIT_ShapeVector it = shapes.begin() + 1; it != shapes.end(); ++it )
    {
        properties = properties.IntersectWith( (*it)->GetProperties() );
    }

    if( properties.nProperties_ & MT_GLDShapeProperties::eText )
    {
        pTextGroup_->show();
        pTextEdit_->setText( properties.strText_ );
        pFontSlider_->setValue( properties.rFontSize_ * 1024 );
    }
    else
        pTextGroup_->hide();

    if( properties.nProperties_ & MT_GLDShapeProperties::eLineColor )
    {
        pLineGroup_->show();
        pLineColorButton_->setPaletteBackgroundColor( QColor( properties.lineColor_ ) );
        pLineWidthSlider_->SetValue( properties.nLineWidth_ );
        pLinePatternCombo_->SetCurrentItem( properties.nLinePattern_ );
    }
    else
        pLineGroup_->hide();

    if( properties.nProperties_ & MT_GLDShapeProperties::eArrowStart )
    {
        pArrowGroup_->show();
        pArrowStartCombo_->SetCurrentItem( properties.nArrowStart_ );
        pArrowEndCombo_->SetCurrentItem( properties.nArrowEnd_ );
    }
    else
        pArrowGroup_->hide();

    if( properties.nProperties_ & MT_GLDShapeProperties::eSize )
    {
        pSizeGroup_->show();
        pSizeSlider_->setValue( properties.rSize_ * 1024 );
    }
    else
        pSizeGroup_->hide();

    if( properties.nProperties_ & MT_GLDShapeProperties::eFillColor )
    {
        pFillGroup_->show();
        pFillColorButton_->setPaletteBackgroundColor( QColor( properties.fillColor_ ) );
        pFillOpacitySlider_->SetValue( (int)(qAlpha( properties.fillColor_ ) / 255.0 * 100.0) );
    }
    else
        pFillGroup_->hide();

    nModifiedProperties_ = MT_GLDShapeProperties::eNone;
    pOkButton_->setEnabled( true );
    pApplyButton_->setEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnApply
// Created: APE 2004-06-28
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnApply()
{
    pApplyButton_->setEnabled( false );

    for( MT_GLDShape_ABC::IT_ShapeVector it = shapes_.begin(); it != shapes_.end(); ++it )
    {
        MT_GLDShapeProperties properties = (*it)->GetProperties();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eText )
            properties.strText_ = pTextEdit_->text();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eTextSize )
            properties.rFontSize_ = pFontSlider_->value() / 1024.0f;

        if( nModifiedProperties_ & MT_GLDShapeProperties::eLineColor )
        {
            QColor color = pLineColorButton_->paletteBackgroundColor();
            properties.lineColor_ = qRgba( color.red(), color.green(), color.blue(), qAlpha( properties.lineColor_ ) );
        }

        if( nModifiedProperties_ & MT_GLDShapeProperties::eLineWidth )
            properties.nLineWidth_ = pLineWidthSlider_->GetValue();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eLinePattern )
            properties.nLinePattern_ = pLinePatternCombo_->GetValue();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eArrowStart )
            properties.nArrowStart_ = pArrowStartCombo_->GetValue();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eArrowEnd )
            properties.nArrowEnd_ = pArrowEndCombo_->GetValue();

        if( nModifiedProperties_ & MT_GLDShapeProperties::eSize )
            properties.rSize_ = pSizeSlider_->value() / 1024.0f;

        if( nModifiedProperties_ & MT_GLDShapeProperties::eFillColor )
        {
            QColor color = pFillColorButton_->paletteBackgroundColor();
            properties.fillColor_ = qRgba( color.red(), color.green(), color.blue(), qAlpha( properties.fillColor_ ) );
        }

        if( nModifiedProperties_ & MT_GLDShapeProperties::eFillOpacity )
        {
            QRgb color = properties.fillColor_;
            properties.fillColor_ = qRgba( qRed( color ), qGreen( color ), qBlue( color ), pFillOpacitySlider_->GetValue() / 100.0 * 255.0 );
        }

        (*it)->SetProperties( properties );
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnText
// Created: APE 2004-07-05
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnText()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eText);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnFontSize
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnFontSize()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eTextSize);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnLineColor
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnLineColor()
{
    QColor color = QColorDialog::getColor( pLineColorButton_->paletteBackgroundColor() );
    if( ! color.isValid() )
        return;

    pApplyButton_->setEnabled( true );
    pLineColorButton_->setPaletteBackgroundColor( color );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eLineColor);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnLineWidth
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnLineWidth()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eLineWidth);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnLinePattern
// Created: APE 2004-07-05
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnLinePattern()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eLinePattern);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnArrowStart
// Created: APE 2004-07-05
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnArrowStart()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eArrowStart);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnArrowEnd
// Created: APE 2004-07-05
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnArrowEnd()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eArrowEnd);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnSize
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnSize()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eSize);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnFillColor
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnFillColor()
{
    QColor color = QColorDialog::getColor( pFillColorButton_->paletteBackgroundColor() );
    if( ! color.isValid() )
        return;

    pApplyButton_->setEnabled( true );
    pFillColorButton_->setPaletteBackgroundColor( color );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eFillColor);
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeDialog::OnFillOpacity
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDShapeDialog::OnFillOpacity()
{
    pApplyButton_->setEnabled( true );
    nModifiedProperties_ = (MT_GLDShapeProperties::E_Properties)(nModifiedProperties_ | MT_GLDShapeProperties::eFillOpacity);
}
