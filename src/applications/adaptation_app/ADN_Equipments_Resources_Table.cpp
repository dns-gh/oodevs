// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Resources_Table.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Equipments_GUI.h"
#include "ADN_Resources_Data.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Table constructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_Table::ADN_Equipments_Resources_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* parent /* = 0 */ )
    : ADN_Table( objectName, connector, parent )
{
    dataModel_.setColumnCount( 1 );

    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Equipments_Resources_Table", "Resources" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    setMaximumHeight( 270 );

    proxyModel_->setDynamicSortFilter( true );
    Sort();
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Table destructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_Table::~ADN_Equipments_Resources_Table()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Table::AddRow
// Created: LGY 2014-01-06
// -----------------------------------------------------------------------------
void ADN_Equipments_Resources_Table::AddRow( int row, void* data )
{
    ADN_Equipments_Data::CategoryInfos* infos = static_cast< ADN_Equipments_Data::CategoryInfos* >( data );
    if( !infos )
        return;
    AddItem( row, 0, data, &infos->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Table::OnContextMenu
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
void ADN_Equipments_Resources_Table::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the dotation list.
    ADN_Resources_Data::T_ResourceInfos_Vector& dotations = ADN_Workspace::GetWorkspace().GetResources().GetData().GetResources();

    // Fill the popup menu with submenus, one for each dotation.
    for( auto it = dotations.begin(); it != dotations.end(); ++it )
    {
        Q3PopupMenu* pSubMenu = new Q3PopupMenu( &targetMenu );

        // Fill the submenu with an entry for each equipement category.
        ADN_Resources_Data::T_CategoryInfos_Vector& categories = (*it)->GetCategories();
        for( auto it2 = categories.begin(); it2 != categories.end(); ++it2 )
        {
            // Avoid offering to add items already present.
            if( Contains( *it2 ) )
                continue;

            // This id is used to encode the category into the item.
            int nItemId = (int)(*it2);
            pSubMenu->insertItem( (*it2)->strName_.GetData().c_str(), nItemId );
        }
        ADN_Tools::SortMenu( *pSubMenu );

        targetMenu.insertItem( ENT_Tr::ConvertFromDotationCategory( (*it)->nType_, ENT_Tr::eToTr ).c_str(), pSubMenu );
    }

    menu.insertItem( tools::translate( "ADN_Equipments_Resources_Table", "Add resource"), &targetMenu ,0 );

    void* data = GetSelectedData();

    if( data != 0 )
        menu.insertItem( tools::translate( "ADN_Equipments_Resources_Table", "Remove resource" ), 1 );

    int nMenuResult = menu.exec(pt);

    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    if( nMenuResult == 1 )
    {
        assert( data != 0 );
        pCTable.RemItem( data );
    }
    else if( nMenuResult > 1 )
    {
        ADN_Resources_Data::CategoryInfo& category = *(ADN_Resources_Data::CategoryInfo*)( nMenuResult );

        ADN_Equipments_Data::CategoryInfos* pNewInfo = new ADN_Equipments_Data::CategoryInfos( category.parentResource_ );
        pNewInfo->SetCrossedElement( &category );
        pNewInfo->rNbr_ = 1;

        pCTable.AddItem( pNewInfo );
        pCTable.AddItem( 0 );

        if( ADN_ListView* listView = ADN_Workspace::GetWorkspace().GetEquipments().GetGui().GetListView() )
            if( ADN_Equipments_Data::EquipmentInfos* infos = static_cast< ADN_Equipments_Data::EquipmentInfos* >( listView->GetCurrentData() ) )
                infos->FillMissingConsumptions();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Table::Contains
// Created: ABR 2012-12-03
// -----------------------------------------------------------------------------
bool ADN_Equipments_Resources_Table::Contains( void* category )
{
    for( int n = 0; dataModel_.item( n, 0 ) != 0; ++n )
    {
        ADN_StandardItem* pItem = static_cast< ADN_StandardItem* >( dataModel_.item( n, 0 ) );
        ADN_Equipments_Data::CategoryInfos* pCategory = static_cast< ADN_Equipments_Data::CategoryInfos* >( pItem->GetData() );
        if( pCategory->GetCrossedElement() == category )
            return true;
    }
    return false;
}
