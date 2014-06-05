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
#include "ADN_Inhabitants_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table constructor
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Consumptions_Table::ADN_Consumptions_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /* = 0 */ )
    : ADN_Table( objectName, connector, pParent )
{
    dataModel_.setColumnCount( 2 );
    QStringList horizontalHeaders;
    horizontalHeaders << tools::translate( "ADN_Consumptions_Table", "Resource network" )
                      << tools::translate( "ADN_Consumptions_Table", "Consumption" );
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
    std::unique_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetSelectedData() != 0;
    ADN_ResourceNetworks_Data::T_ResourceNetworkInfosVector& vAllResources = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos();
    for( auto it = vAllResources.begin(); it != vAllResources.end(); ++it )
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
        pMenu->insertItem( tools::translate( "ADN_Consumptions_Table", "Add resource" ), pTargetMenu.get(), 0 );
    if( bDisplayRem )
        pMenu->insertItem( tools::translate( "ADN_Consumptions_Table", "Remove resource"), 1 );
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
    ADN_Inhabitants_Data::InhabitantsInfosConsumption* pNewInfo = new ADN_Inhabitants_Data::InhabitantsInfosConsumption();
    pNewInfo->SetCrossedElement( ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos()[ resource ] );
    pNewInfo->consumption_ = 0;
    ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
    pCTable.AddItem( pNewInfo );
    pCTable.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::RemoveCurrentConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::RemoveCurrentConsumption()
{
    ADN_Inhabitants_Data::InhabitantsInfosConsumption* pCurResource = static_cast< ADN_Inhabitants_Data::InhabitantsInfosConsumption* >( GetSelectedData() );
    if( pCurResource )
    {
        // remove current data from list
        // take care cause pCurData_ can change!!
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurResource );
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
        ADN_Inhabitants_Data::InhabitantsInfosConsumption* pInfos = static_cast< ADN_Inhabitants_Data::InhabitantsInfosConsumption* >( pItem->GetData() );
        if( pInfos->GetCrossedElement() == &infos )
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
    ADN_Inhabitants_Data::InhabitantsInfosConsumption* pCurResource = static_cast< ADN_Inhabitants_Data::InhabitantsInfosConsumption* >( data );
    if( !pCurResource )
        return;

    AddItem( row, 0, data, &pCurResource->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pCurResource->consumption_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
}
