// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-07-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MT_SpinTableItem.cpp $
// $Author: Sbo $
// $Modtime: 29/07/05 11:12 $
// $Revision: 1 $
// $Workfile: MT_SpinTableItem.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MT_SpinTableItem.h"

using namespace MT;


// -----------------------------------------------------------------------------
// Name: MT_SpinTableItem constructor
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
MT_SpinTableItem::MT_SpinTableItem( QTable* pTable )
    : QTableItem ( pTable, QTableItem::WhenCurrent )
    , pSpinBox_  ( 0 )
    , nMinValue_ ( 0  ) //-|
    , nMaxValue_ ( 99 ) //-| default values from QRangeControl used by QSpinBox
    , nStep_     ( 1  ) //-|
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_SpinTableItem constructor
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
MT_SpinTableItem::MT_SpinTableItem( QTable* pTable, int nMinValue, int nMaxValue, int nStep /*= 1*/ )
    : QTableItem ( pTable, QTableItem::WhenCurrent )
    , pSpinBox_  ( 0         )
    , nMinValue_ ( nMinValue )
    , nMaxValue_ ( nMaxValue )
    , nStep_     ( nStep     )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MT_SpinTableItem destructor
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
MT_SpinTableItem::~MT_SpinTableItem()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MT_SpinTableItem::createEditor
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
QWidget* MT_SpinTableItem::createEditor() const
{
    QString strValue = text();
    const_cast< MT_SpinTableItem * >( this )->pSpinBox_ = new QSpinBox( nMinValue_, nMaxValue_, nStep_, table()->viewport(), "mt_spintableitem" );
    QObject::connect( pSpinBox_, SIGNAL( valueChanged( int ) ), table(), SLOT( doValueChanged() ) );
	
    if( !strValue.isNull() )
        pSpinBox_->setValue( strValue.toInt() );
    else
        pSpinBox_->setValue( 0 );

    return pSpinBox_;
}

// -----------------------------------------------------------------------------
// Name: MT_SpinTableItem::setContentFromEditor
// Created: SBO 2005-07-29
// -----------------------------------------------------------------------------
void MT_SpinTableItem::setContentFromEditor( QWidget* pWidget )
{
    QSpinBox* pSpinBox = static_cast< QSpinBox* >( pWidget );
    if( pSpinBox )
        setText( pSpinBox->text() );
    else
        QTableItem::setContentFromEditor( pWidget );
}