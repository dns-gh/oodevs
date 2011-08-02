// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GuiBuilder.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 14:49 $
// $Revision: 8 $
// $Workfile: ADN_GuiBuilder.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GuiBuilder.h"

#include "ADN_Type_ABC.h"
#include "ADN_FileChooser.h"
#include "ADN_Project_Data.h"
#include "ADN_Table.h"
#include "ADN_Gfx_ABC.h"
#include "ADN_Validator.h"

#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlineedit.h>
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder constructor
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_GuiBuilder::ADN_GuiBuilder()
: pCurrentFieldWidget1_ ( 0 )
, pCurrentFieldWidget2_ ( 0 )
, pCurrentFieldGfx2_    ( 0 )
, pCurrentFieldWidget3_ ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder destructor
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
ADN_GuiBuilder::~ADN_GuiBuilder()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddFieldHolder
// Created: APE 2005-03-23
// -----------------------------------------------------------------------------
QWidget* ADN_GuiBuilder::AddFieldHolder( QWidget* pParent )
{
    QWidget* pHolder = new QWidget( pParent );
    new Q3GridLayout( pHolder, 0, 3, 5, 5 );
    return pHolder;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddFileField
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
ADN_FileChooser* ADN_GuiBuilder::AddFileField( QWidget* pParent, const char* szName, ADN_Connector_ABC*& pGuiConnector, const char* szFilter /* = "(*.*)"*/ )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_FileChooser* pChooser = new ADN_FileChooser( pParent, szFilter );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pChooser;
    pCurrentFieldGfx2_ = 0;
    pCurrentFieldWidget3_ = 0;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pChooser, 0 );

    pGuiConnector = &pChooser->GetConnector(ADN_FileChooser::eFile);
    pChooser->GetConnector(ADN_FileChooser::eDirectory).Connect( &ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory() );
    return pChooser;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddFileField
// Created: APE 2005-03-23
// -----------------------------------------------------------------------------
ADN_FileChooser* ADN_GuiBuilder::AddFileField( QWidget* pParent, const char* szName, ADN_Type_String& strFileNameConnector, const char* szFilter )
{
    // Create the field and labels.
    QLabel* pNameLabel = new QLabel( szName, pParent );
    ADN_FileChooser* pChooser = new ADN_FileChooser( pParent, szFilter );

    pCurrentFieldWidget1_ = pNameLabel;
    pCurrentFieldWidget2_ = pChooser;
    pCurrentFieldGfx2_ = 0;
    pCurrentFieldWidget3_ = 0;

    // Lay them out if necessary.
    this->DoFieldLayout( pParent, pNameLabel, pChooser, 0 );

    pChooser->GetConnector(ADN_FileChooser::eFile).Connect( &strFileNameConnector );
    pChooser->GetConnector(ADN_FileChooser::eDirectory).Connect( &ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory() );
    return pChooser;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetToolTip
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetToolTip( const char* szToolTip )
{
    if( pCurrentFieldWidget1_ != 0 )
        QToolTip::add( pCurrentFieldWidget1_, szToolTip );
    if( pCurrentFieldWidget2_ != 0 )
        QToolTip::add( pCurrentFieldWidget2_, szToolTip );
    if( pCurrentFieldWidget3_ != 0 )
        QToolTip::add( pCurrentFieldWidget3_, szToolTip );
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetValidator
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetValidator( QValidator* pValidator )
{
    if( pCurrentFieldWidget2_ == 0 || ! pCurrentFieldWidget2_->inherits( "QLineEdit" ) )
        return;

    ((QLineEdit*)pCurrentFieldWidget2_)->setValidator( pValidator );
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::SetEnabled
// Created: APE 2005-04-05
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::SetEnabled( bool bEnabled )
{
    if( pCurrentFieldWidget2_ == 0 || pCurrentFieldGfx2_ == 0 )
        return;

    // Expects that the previously created field inherited from ADN_Gfx_ABC
    pCurrentFieldGfx2_->SetAutoEnabled( bEnabled );
    pCurrentFieldWidget2_->setEnabled( bEnabled );
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::DoFieldLayout
// Created: APE 2005-03-11
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::DoFieldLayout( QWidget* pParent, QWidget* pWidget1, QWidget* pWidget2, QWidget* pWidget3 )
{
    // Lay them out if necessary.
    assert( pParent->layout() != 0 );
    if( pParent->layout()->inherits( "QGridLayout" ) )
    {
        Q3GridLayout* pLayout = (Q3GridLayout*)pParent->layout();
        int nRow = pLayout->numRows();
        pLayout->setColStretch( 0, 0 );
        pLayout->setColStretch( 1, 500 );
        pLayout->setColStretch( 2, 0 );
        if( pWidget3 != 0 )
        {
            pLayout->addWidget( pWidget1, nRow, 0 );
            pLayout->addWidget( pWidget2, nRow, 1 );
            pLayout->addWidget( pWidget3, nRow, 2 );
        }
        else
        {
            pLayout->addWidget( pWidget1, nRow, 0 );
            pLayout->addMultiCellWidget( pWidget2, nRow, nRow, 1, 2 );
        }
    }
    else if( pParent->inherits( "QGroupBox" ) )
    {
        if( pWidget3 == 0 )
            ((Q3GroupBox*)pParent)->addSpace( 0 );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddStretcher
// Created: APE 2005-03-25
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::AddStretcher( QWidget* pParent, Qt::Orientation nOrientation )
{
    QSpacerItem* pSpacer = new QSpacerItem( 1, 1, (nOrientation == Qt::Vertical) ? QSizePolicy::Minimum : QSizePolicy::Expanding, (nOrientation == Qt::Vertical) ? QSizePolicy::Expanding : QSizePolicy::Minimum );
    pParent->layout()->addItem( pSpacer );
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddStretcher
// Created: APE 2005-03-25
// -----------------------------------------------------------------------------
void ADN_GuiBuilder::AddStretcher( QLayout* pLayout, Qt::Orientation nOrientation )
{
    QSpacerItem* pSpacer = new QSpacerItem( 1, 1, (nOrientation == Qt::Vertical) ? QSizePolicy::Minimum : QSizePolicy::Expanding, (nOrientation == Qt::Vertical) ? QSizePolicy::Expanding : QSizePolicy::Minimum );
    pLayout->addItem( pSpacer );
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::CreateTable
// Created: APE 2005-03-29
// -----------------------------------------------------------------------------
ADN_Table* ADN_GuiBuilder::CreateTable( QWidget* pParent )
{
    ADN_Table* pTable = new ADN_Table( pParent );

    // Selection and sorting.
    pTable->setSorting( true );
    pTable->setSelectionMode( Q3Table::NoSelection );
    pTable->setShowGrid( true );

    // Hide the vertical header.
    pTable->verticalHeader()->hide();
    pTable->setLeftMargin( 0 );

    return pTable;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddTableCell
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
Q3TableItem* ADN_GuiBuilder::AddTableCell( ADN_Table* pTable, int nRow, int nCol, const char* strText, Q3TableItem::EditType nEditType )
{
    Q3TableItem* pItem = new Q3TableItem( pTable, nEditType, strText );
    pTable->setItem( nRow, nCol, pItem );
    return pItem;
}


// -----------------------------------------------------------------------------
// Name: ADN_GuiBuilder::AddTableCell
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
Q3TableItem* ADN_GuiBuilder::AddTableCell( ADN_Table* pTable, int nRow, int nCol, int nRowSpan, int nColSpan, const char* strText, Q3TableItem::EditType nEditType )
{
    Q3TableItem* pItem = new Q3TableItem( pTable, nEditType, strText );
    pTable->setItem( nRow, nCol, pItem );
    pItem->setSpan( nRowSpan, nColSpan );
    return pItem;
}



