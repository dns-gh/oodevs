// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdowns_PartsTable.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 6 $
// $Workfile: ADN_Breakdowns_PartsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_PartsTable.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Breakdowns_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_PartsTable::ADN_Breakdowns_PartsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Name" ) << tr( "Nbr required" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1, 0, std::numeric_limits< int >::max() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable destructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_PartsTable::~ADN_Breakdowns_PartsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::AddRow
// Created: JSR 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Breakdowns_PartsTable::AddRow( int row, void* data )
{
    ADN_Breakdowns_Data::RepairPartInfo* pInfos = static_cast< ADN_Breakdowns_Data::RepairPartInfo* >( data );
    if( !pInfos )
        return;
    AddItem( row, 0, data, &pInfos->ptrPart_.GetData()->strName_, ADN_StandardItem::eString );
    AddItem( row, 1, data, &pInfos->nNbr_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_Breakdowns_PartsTable::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu subMenu( &menu );

    ADN_Equipement_Data::T_CategoryInfos_Vector& parts = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Piece ).categories_;
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector it = parts.begin(); it != parts.end(); ++it )
    {
        if( Contains( **it ) )
            continue;
        subMenu.insertItem( ( *it )->strName_.GetData().c_str(), 1 + static_cast< int >( std::distance( parts.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( subMenu );

    menu.insertItem( tr( "Add required part"), &subMenu );
    menu.insertItem( tr( "Remove required part"), 0 );
    menu.setItemEnabled( 0, GetSelectedData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 0 )
    {
        // Delete the current element.
        ADN_Breakdowns_Data::RepairPartInfo* pCurrentPart = reinterpret_cast< ADN_Breakdowns_Data::RepairPartInfo* >( GetSelectedData() );
        if( pCurrentPart != 0 )
            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurrentPart );
    }
    else
    {
        // Create a new element
        ADN_Breakdowns_Data::RepairPartInfo* pNewInfo = new ADN_Breakdowns_Data::RepairPartInfo();
        pNewInfo->ptrPart_ = parts[ nMenuResult - 1 ];
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Breakdowns_PartsTable::Contains( ADN_Equipement_Data::CategoryInfo& category )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        const QModelIndex index = dataModel_.index( row, 1 );
        ADN_Breakdowns_Data::RepairPartInfo* pInfos = static_cast<ADN_Breakdowns_Data::RepairPartInfo*>( GetDataFromIndex( index ) );
        if( pInfos->ptrPart_.GetData() == &category )
            return true;
    }
    return false;
}
