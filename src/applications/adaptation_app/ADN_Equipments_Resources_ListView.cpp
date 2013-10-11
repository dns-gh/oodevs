// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Resources_ListView.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_StandardItem.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Equipments_GUI.h"
#include "ADN_Resources_Data.h"
#include "ADN_WorkspaceElement.h"
#include "ENT/ENT_Tr.h"

namespace
{
    class ADN_Connector_ListView_Equipment_Resource : public ADN_Connector_ListView_ABC
        , private boost::noncopyable
    {
    public:
        explicit ADN_Connector_ListView_Equipment_Resource( ADN_ListView& list ) : ADN_Connector_ListView_ABC( list ) {}
        virtual ~ADN_Connector_ListView_Equipment_Resource() {}

        ADN_StandardItem* CreateItem( void* pObj )
        {
            // Create the new list item.
            ADN_StandardItem* pItem = new ADN_StandardItem( pObj );

            // Connect list item with object's name
            ADN_Equipments_Data::CategoryInfos* infos = static_cast< ADN_Equipments_Data::CategoryInfos* >( pObj );
            if( infos && infos->GetCrossedElement() )
                pItem->Connect( &infos->GetCrossedElement()->strName_ );
            return pItem;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView constructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_ListView::ADN_Equipments_Resources_ListView( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* parent /* = 0 */ )
    : ADN_ListView( parent, objectName , tools::translate( "ADN_Equipments_Resources_ListView", "Resources" ) )
{
    pConnector_ = new ADN_Connector_ListView_Equipment_Resource( *this );
    connector = pConnector_;

    setAlternatingRowColors( true );

    proxyModel_->setDynamicSortFilter( true );
    Sort();
    setSortingEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView destructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_ListView::~ADN_Equipments_Resources_ListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView::ConnectItem
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
void ADN_Equipments_Resources_ListView::ConnectItem( bool /*bConnect*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView::OnContextMenu
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
void ADN_Equipments_Resources_ListView::OnContextMenu( const QPoint& pt )
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

        targetMenu.insertItem( ENT_Tr::ConvertFromDotationFamily( (*it)->nType_, ENT_Tr_ABC::eToTr ).c_str(), pSubMenu );
    }

    menu.insertItem( tools::translate( "ADN_Equipments_Resources_ListView", "Add resource"), &targetMenu ,0 );

    if( pCurData_ != 0 )
        menu.insertItem( tools::translate( "ADN_Equipments_Resources_ListView", "Remove resource" ), 1 );

    int nMenuResult = menu.exec(pt);

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    if( !pCTable )
        return;

    if( nMenuResult == 1 )
    {
        assert( pCurData_ != 0 );
        pCTable->RemItem( pCurData_ );
    }
    else if( nMenuResult > 1 )
    {
        ADN_Resources_Data::CategoryInfo& category = *(ADN_Resources_Data::CategoryInfo*)( nMenuResult );

        ADN_Equipments_Data::CategoryInfos* pNewInfo = new ADN_Equipments_Data::CategoryInfos( category.parentResource_ );
        pNewInfo->SetCrossedElement( &category );
        pNewInfo->rNbr_ = 1;

        pCTable->AddItem( pNewInfo );
        pCTable->AddItem( 0 );

        if( ADN_ListView* listView = ADN_Workspace::GetWorkspace().GetEquipments().GetGui().GetListView() )
            if( ADN_Equipments_Data::EquipmentInfos* infos = static_cast< ADN_Equipments_Data::EquipmentInfos* >( listView->GetCurrentData() ) )
                infos->FillMissingConsumptions();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView::ADN_Equipments_Resources_ListView::Contains
// Created: ABR 2012-12-03
// -----------------------------------------------------------------------------
bool ADN_Equipments_Resources_ListView::Contains( void* category )
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

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_ListView::GetToolTipFor
// Created: ABR 2012-12-03
// -----------------------------------------------------------------------------
std::string ADN_Equipments_Resources_ListView::GetToolTipFor( const QModelIndex& index )
{
    if( index.isValid() )
    {
        void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
        ADN_Equipments_Data::CategoryInfos* pCastData = static_cast< ADN_Equipments_Data::CategoryInfos* >( pData );
        assert( pCastData != 0 );
        if( pCastData->GetCrossedElement() )
            return pCastData->GetCrossedElement()->strName_.GetData();
    }
    return "";
}
