// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Breakdowns_PartsTable.h"
#include "ADN_Resources_Data.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_WorkspaceElement.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_PartsTable::ADN_Breakdowns_PartsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    setShowGrid( false );
    dataModel_.setColumnCount( 2 );
    verticalHeader()->setVisible( false );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Breakdowns_PartsTable", "Name" ) << tools::translate( "ADN_Breakdowns_PartsTable", "Nbr required" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    delegate_.AddSpinBoxOnColumn( 1, 1, std::numeric_limits< int >::max() );
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
    AddItem( row, 0, data, &pInfos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
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

    ADN_Resources_Data::T_CategoryInfos_Vector& parts = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( sword::dotation_type_parts ).categories_;
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        if( Contains( **it ) )
            continue;
        subMenu.insertItem( ( *it )->strName_.GetData().c_str(), 1 + static_cast< int >( std::distance( parts.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( subMenu );

    menu.insertItem( tools::translate( "ADN_Breakdowns_PartsTable", "Add required part"), &subMenu );
    menu.insertItem( tools::translate( "ADN_Breakdowns_PartsTable", "Remove required part"), 0 );
    menu.setItemEnabled( 0, GetSelectedData() != 0 );

    int nMenuResult = menu.exec( pt );

    if( nMenuResult == -1 )
        return;
    else if( nMenuResult == 0 )
    {
        // Delete the current element.
        ADN_Breakdowns_Data::RepairPartInfo* pCurrentPart = reinterpret_cast< ADN_Breakdowns_Data::RepairPartInfo* >( GetSelectedData() );
        if( pCurrentPart != 0 )
            static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurrentPart );
    }
    else
    {
        // Create a new element
        ADN_Breakdowns_Data::RepairPartInfo* pNewInfo = new ADN_Breakdowns_Data::RepairPartInfo();
        pNewInfo->SetCrossedElement( parts[ nMenuResult - 1 ] );
        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        pCTable.AddItem( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_PartsTable::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Breakdowns_PartsTable::Contains( ADN_Resources_Data::CategoryInfo& category )
{
    for( int row = 0; row < dataModel_.rowCount(); ++row )
    {
        const QModelIndex index = dataModel_.index( row, 1 );
        ADN_Breakdowns_Data::RepairPartInfo* pInfos = static_cast<ADN_Breakdowns_Data::RepairPartInfo*>( GetDataFromIndex( index ) );
        if( pInfos->GetCrossedElement() == &category )
            return true;
    }
    return false;
}
