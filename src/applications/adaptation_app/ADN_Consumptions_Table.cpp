// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Consumptions_Table.h"
#include "ADN_People_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table constructor
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Consumptions_Table::ADN_Consumptions_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table3( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Resource network" )
                      << tr( "Consumption" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddSpinBoxOnColumn( 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table destructor
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Consumptions_Table::~ADN_Consumptions_Table()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::OnContextMenu
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::OnContextMenu( const QPoint& pt )
{
    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetSelectedData() != 0;
    ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& vAllResources = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos();
    for( ADN_ResourceNetworks_Data::IT_ResourceNetworkInfosVector it = vAllResources.begin(); it != vAllResources.end(); ++it )
    {
        if( this->Contains( **it ) )
            continue;
        bDisplayAdd = true;
        pTargetMenu->insertItem( ( *it )->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vAllResources.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( *pTargetMenu );
    if( !bDisplayAdd && !bDisplayRem )
        return;
    Q3PopupMenu* pMenu = new Q3PopupMenu( this );
    if( bDisplayAdd )
        pMenu->insertItem( tr( "Add resource" ), pTargetMenu.get(), 0 );
    if( bDisplayRem )
        pMenu->insertItem( tr( "Remove resource"), 1 );
    int nMenu = pMenu->exec( pt );
    if( nMenu == 1 )
        RemoveCurrentConsumption();
    else if( nMenu > 1 )
    {
        assert( nMenu - 2 < static_cast< int >( vAllResources.size() ) );
        CreateNewConsumption( nMenu - 2 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::CreateNewConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::CreateNewConsumption( int resource )
{
    ADN_People_Data::PeopleInfosConsumption* pNewInfo = new ADN_People_Data::PeopleInfosConsumption();
    pNewInfo->ptrResource_ = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos()[ resource ];
    pNewInfo->consumption_ = 0;
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::RemoveCurrentConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::RemoveCurrentConsumption()
{
    ADN_People_Data::PeopleInfosConsumption* pCurResource = static_cast< ADN_People_Data::PeopleInfosConsumption* >( GetSelectedData() );
    if( pCurResource )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurResource );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::Contains
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
bool ADN_Consumptions_Table::Contains( const ADN_ResourceNetworks_Data::ResourceNetworkInfos& infos ) const
{
    int n = 0;
    while( ADN_StandardItem* pItem =  static_cast< ADN_StandardItem* >( dataModel_.item( n, 1 ) ) )
    {
        ADN_People_Data::PeopleInfosConsumption* pInfos = static_cast< ADN_People_Data::PeopleInfosConsumption* >( pItem->GetData() );
        if( pInfos->ptrResource_.GetData() == &infos )
            return true;
        ++n;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::AddRow
// Created: NPT 2012-11-05
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::AddRow( int row, void* data )
{
    ADN_People_Data::PeopleInfosConsumption* pCurResource = static_cast< ADN_People_Data::PeopleInfosConsumption* >( data );
    if( !pCurResource )
        return;

    AddItem( row, 0, data, QString( pCurResource->GetItemName().c_str() ), Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pCurResource->consumption_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
