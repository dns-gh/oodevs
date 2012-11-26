// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_BreakdownsTable.h"
#include "moc_ADN_Equipments_BreakdownsTable.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Equipments_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Workspace.h"

typedef ADN_Equipments_Data::BreakdownInfos BreakdownInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_BreakdownsTable constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_BreakdownsTable::ADN_Equipments_BreakdownsTable( const QString& objectName, const QString& name, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0 */ )
:   ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << name
                      << tr( "Odds (%)" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.SetSingleColumnSumRestriction( 1, gui::CommonDelegate::eLTE, 100. );
    delegate_.AddDoubleSpinBoxOnColumn( 1, 0.0, 100.0, 0.01, 2 );
    setMinimumHeight( 150 );
    setMaximumHeight( 150 );
    setShowGrid( false );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_BreakdownsTable destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_BreakdownsTable::~ADN_Equipments_BreakdownsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_BreakdownsTable::OnContextMenu
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_BreakdownsTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );

    ADN_Breakdowns_Data& breakdownsData = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData();
    for( ADN_Breakdowns_Data::IT_BreakdownInfoVector it = breakdownsData.vBreakdowns_.begin(); it != breakdownsData.vBreakdowns_.end(); ++it )
        addMenu.insertItem( (*it)->strName_.GetData().c_str(), 2 + static_cast< int >( std::distance( breakdownsData.vBreakdowns_.begin(), it ) ) );

    menu.insertItem( tr( "New" ), &addMenu );
    menu.insertItem( tr( "Delete" ), 1 );
    menu.setItemEnabled( 1, GetSelectedData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 1 )
    {
        // Delete the current element.
        BreakdownInfos* pCurrent = static_cast< BreakdownInfos* >( GetSelectedData() );
        if( pCurrent != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrent );
    }
    else
    {
        // Create a new element
        ADN_Breakdowns_Data::BreakdownInfo* pCast = breakdownsData.vBreakdowns_[ nMenuResult - 2 ];
        BreakdownInfos* pNewInfo = new BreakdownInfos();
        pNewInfo->ptrBreakdown_ = pCast;
        pNewInfo->ptrBreakdown_.SetVector( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().vBreakdowns_ );
        //pNewInfo->ptrBreakdown_.SetVector( pCast->nti_.vBreakdowns_ );
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_BreakdownsTable::dataChanged
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
void ADN_Equipments_BreakdownsTable::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
    ADN_Table::dataChanged( topLeft, bottomRight );

    double rSum = 0;
    int i = 0;
    while( dataModel_.item( i, 1 ) != 0 )
    {
        bool bOk = false;
        double r = dataModel_.item( i, 1 )->text().toDouble( &bOk ) ;
        if( bOk )
            rSum += r;
        ++i;
    }

    if( rSum == 100.0 )
    {
        QColorGroup cg = this->horizontalHeader()->colorGroup();
        cg.setColor( QColorGroup::ButtonText, Qt::black );
        QPalette palette( this->horizontalHeader()->palette() );
        palette.setActive( cg );
        palette.setDisabled( cg );
        palette.setInactive( cg );
        this->horizontalHeader()->setPalette( palette );
    }
    else
    {
        QColorGroup cg = this->horizontalHeader()->colorGroup();
        cg.setColor( QColorGroup::ButtonText, Qt::red );
        QPalette palette( this->horizontalHeader()->palette() );
        palette.setActive( cg );
        palette.setDisabled( cg );
        palette.setInactive( cg );
        this->horizontalHeader()->setPalette( palette );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_BreakdownsTable::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Equipments_BreakdownsTable::AddRow( int row, void* data )
{
    BreakdownInfos* pBreakdown = static_cast<BreakdownInfos*>( data );
    if( !pBreakdown )
        return;
    AddItem( row, 0, data, &pBreakdown->ptrBreakdown_.GetData()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pBreakdown->rPercentage_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
