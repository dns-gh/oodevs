// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Dotations_GUI.h"
#include "moc_ADN_Equipments_Dotations_GUI.cpp"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipments_Data::CategoryInfos CategoryInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI constructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Dotations_GUI::ADN_Equipments_Dotations_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */, int visibleColumns /*= eColumn_All*/, QHeaderView::ResizeMode resizeMode /* = QHeaderView::Stretch */ )
    : ADN_Table( objectName, connector, pParent )
{
    // Setup the columns.
    dataModel_.setColumnCount( 4 );

    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Supplies" )
                      << tr( "Qty" )
                      << tr( "Log threshold (%)" )
                      << tr( "Normalized consumption" );

    delegate_.AddSpinBoxOnColumn( 1, 1, INT_MAX );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0.0, 100.0, 0.01, 2 );
    delegate_.AddDoubleSpinBoxOnColumn( 3, 0.001, INT_MAX, 0.001, 3 );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( resizeMode );
    verticalHeader()->setVisible( false );
    setMaximumHeight( 270 );

    proxyModel_->setDynamicSortFilter( true );
    Sort();
    setSortingEnabled( false );

    if( !( visibleColumns & eColumn_Category ) )
        hideColumn( 0 );
    if( !( visibleColumns & eColumn_Quantity ) )
        hideColumn( 1 );
    if( !( visibleColumns & eColumn_Threshold ) )
        hideColumn( 2 );
    if( !( visibleColumns & eColumn_Consumption ) )
        hideColumn( 3 );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_Dotations_GUI::~ADN_Equipments_Dotations_GUI()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Equipments_Dotations_GUI::OnContextMenu( const QPoint& pt )
{
    if( isColumnHidden( 0 ) )
        return;

    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the dotation list.
    ADN_Resources_Data::T_ResourceInfos_Vector& dotations
        = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources();

    // Fill the popup menu with submenus, one for each dotation.
    for( auto it = dotations.begin(); it != dotations.end(); ++it )
    {
        Q3PopupMenu* pSubMenu = new Q3PopupMenu( &targetMenu );

        // Fill the submenu with an entry for each equipement category.
        ADN_Resources_Data::T_CategoryInfos_Vector& categories = (*it)->GetCategories();
        for( auto it2 = categories.begin(); it2 != categories.end(); ++it2 )
        {
            // Avoid offering to add items already present.
            if( Contains( **it2 ) )
                continue;

            // This id is used to encode the category into the item.
            int nItemId = (int)(*it2);
            pSubMenu->insertItem( (*it2)->strName_.GetData().c_str(), nItemId );
        }
        ADN_Tools::SortMenu( *pSubMenu );

        targetMenu.insertItem( ENT_Tr::ConvertFromDotationType( (*it)->nType_, ENT_Tr::eToTr ).c_str(), pSubMenu );
    }

    menu.insertItem( tr( "Add supplies"), &targetMenu ,0 );
    if( GetSelectedData() != 0 )
        menu.insertItem( tr( "Remove supplies" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentDotation();
    else if( nMenuResult > 1 )
        AddNewDotation( *(ADN_Resources_Data::CategoryInfo*)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Equipments_Dotations_GUI::Contains( ADN_Resources_Data::CategoryInfo& category )
{
    int n = 0;
    while( dataModel_.item( n, 1 ) != 0 )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem* >( dataModel_.item( n, 1 ) );
        CategoryInfos* pCategory = static_cast< CategoryInfos* >( pItem->GetData() );
        if( pCategory->GetCrossedElement() == &category )
            return true;
        ++n;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI::AddNewDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Equipments_Dotations_GUI::AddNewDotation( ADN_Resources_Data::CategoryInfo& category )
{
    CategoryInfos* pNewInfo = new CategoryInfos( category.parentResource_ );
    pNewInfo->SetCrossedElement( &category );
    pNewInfo->rNbr_ = 1;

    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Dotations_GUI::RemoveCurrentDotation
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Equipments_Dotations_GUI::RemoveCurrentDotation()
{
    assert( GetSelectedData() != 0 );
    static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( GetSelectedData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Equipments_Dotations_GUI::AddRow( int row, void* data )
{
    CategoryInfos* pCategory = static_cast< CategoryInfos* >( data );
    if( !pCategory )
        return;
    AddItem( row, 0, data, &pCategory->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pCategory->rNbr_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
    AddItem( row, 2, data, &pCategory->rLogThreshold_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
    AddItem( row, 3, data, &pCategory->rNormalizedConsumption_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
