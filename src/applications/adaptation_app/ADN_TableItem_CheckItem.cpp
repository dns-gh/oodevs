// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_CheckItem.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 15:46 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_CheckItem.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TableItem_CheckItem.h"

#include "ADN_Connector_Bool.h"
#include "ADN_BoolEditor.h"

int ADN_TableItem_CheckItem::RTTI = 222;

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem constructor
/** @param  parent
    @param  data
*/
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
ADN_TableItem_CheckItem::ADN_TableItem_CheckItem(ADN_Table* parent,void *data)
: ADN_TableItem_ABC( parent, data )
, bIsChecked_( false )
, pEditor_( 0 )
{
    pConnector_=new ADN_Connector_Bool< ADN_TableItem_CheckItem >(this);
    setReplaceable( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem destructor
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
ADN_TableItem_CheckItem::~ADN_TableItem_CheckItem()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::*createEditor
/** @return
*/
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
QWidget* ADN_TableItem_CheckItem::createEditor() const
{
    ((ADN_TableItem_CheckItem*)(this))->pEditor_ = new ADN_BoolEditor( table()->viewport(), bIsChecked_ );
    pEditor_->SetState( bIsChecked_ );
    QObject::connect( pEditor_, SIGNAL( StateChanged( bool ) ), table(), SLOT( doValueChanged() ) );

    return pEditor_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::setContentFromEditor
/** @param  w
*/
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
void ADN_TableItem_CheckItem::setContentFromEditor( QWidget *w )
{
    if( w->inherits( "ADN_BoolEditor" ) )
    {
        ADN_BoolEditor* pEditor = ( ADN_BoolEditor* )w;

        setChecked( pEditor->GetState() );
    }
    else
    if( w->inherits( "QCheckBox" ) )
    {
        QCheckBox* pEditor = ( QCheckBox* )w;
        setChecked( pEditor->isChecked() );
    }
    else
        Q3TableItem::setContentFromEditor( w );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::paint
/** @param  p
    @param  cg
    @param  cr
    @param  selected
*/
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
void ADN_TableItem_CheckItem::paint( QPainter *pPainter, const QColorGroup &cg, const QRect &rc, bool bSelected )
{
    Q3TableItem::paint( pPainter, cg, rc, bSelected );
    QSize sz = QSize( table()->style()->pixelMetric( QStyle::PM_IndicatorWidth )
                    , table()->style()->pixelMetric( QStyle::PM_IndicatorHeight ) );
    pPainter->fillRect( ( rc.width() - sz.width() ) / 2 - 2
                      , ( rc.height() - sz.height() ) / 2 - 2
                      , sz.width() + 4
                      , sz.height() + 4
                      , cg.brush( QColorGroup::Text ) );

    QStyleOption* opt = new QStyleOption();
    opt->rect = QRect( ( rc.width() - sz.width() ) / 2, ( rc.height() - sz.height() ) / 2, sz.width(), sz.height() );
    opt->state = bIsChecked_ ? ( QStyle::State_Enabled | QStyle::State_On ) : QStyle::State_Off;
    table()->style()->drawPrimitive( QStyle::PE_IndicatorCheckBox, opt, pPainter );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::setChecked
/** @param  b
    @return
*/
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
void ADN_TableItem_CheckItem::setChecked( bool b )
{
    bIsChecked_ = b;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::setHidden
// Created: MCO 2011-07-01
// -----------------------------------------------------------------------------
void ADN_TableItem_CheckItem::setHidden( bool )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::DoValueChanged
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
void ADN_TableItem_CheckItem::DoValueChanged()
{
    static_cast<ADN_Connector_Bool< ADN_TableItem_CheckItem >*>(pConnector_)->SetDataChanged( pEditor_->GetState() );
}
