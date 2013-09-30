// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Resources_AttritionTable.h"
#include "moc_ADN_Resources_AttritionTable.cpp"
#include "ADN_Resources_Data.h"
#include "ADN_Resources_GUI.h"

//-----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionTable constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Resources_AttritionTable::ADN_Resources_AttritionTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 4 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Armor" )
                      << tr( "% on site fixable" )
                      << tr( "% maintenance support needed" )
                      << tr( "% destroyed" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );
    verticalHeader()->setVisible( false );
    setMinimumHeight( 200 );

    delegate_.AddColorOnColumn( 1, 0., 100. );
    delegate_.AddColorOnColumn( 2, 0., 100. );
    delegate_.AddColorOnColumn( 3, 0., 100. );
    delegate_.AddDoubleSpinBoxOnColumn( 1 );
    delegate_.AddDoubleSpinBoxOnColumn( 2 );
    delegate_.AddDoubleSpinBoxOnColumn( 3 );
    std::vector< int > linkedList;
    linkedList.push_back( 1 );
    linkedList.push_back( 2 );
    linkedList.push_back( 3 );
    delegate_.SetColumnsSumRestriction( linkedList, gui::CommonDelegate::eLTE, 100. );
}

//-----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionTable destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Resources_AttritionTable::~ADN_Resources_AttritionTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionTable::AddRow
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionTable::AddRow( int row, void* data )
{
    helpers::AttritionInfos* pAttrition = static_cast< helpers::AttritionInfos* >( data );
    if( !pAttrition )
        return;
    AddItem( row, 0, data, &pAttrition->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pAttrition->rRepairNoEvac_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pAttrition->rRepairWithEvac_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pAttrition->rDestroy_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_AttritionTable::dataChanged
// Created: JSR 2012-10-31
// -----------------------------------------------------------------------------
void ADN_Resources_AttritionTable::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );
    if( topLeft == bottomRight )
        ADN_Workspace::GetWorkspace().GetResources().GetGui().UpdateGraph();
}
