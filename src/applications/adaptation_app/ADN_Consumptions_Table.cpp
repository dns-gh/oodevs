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
#include "ADN_Connector_Table_ABC.h"
#include "ADN_People_Data.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Consumptions_Table_Connector
    : public ADN_Connector_Table_ABC
{
public:
    explicit ADN_Consumptions_Table_Connector( ADN_Consumptions_Table& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {
        // NOTHING
    }

    void AddSubItems( int i, void* obj )
    {
        assert(obj);
        ADN_TableItem_String* pItemCombo = 0;
        ADN_TableItem_Int* pItemInt = 0;
        // add a new row & set new values
        tab_.setItem( i, 0, pItemCombo = new ADN_TableItem_String( &tab_, obj, Q3TableItem::Never ) );
        tab_.setItem( i, 1, pItemInt = new ADN_TableItem_Int( &tab_, obj ) );
        tab_.adjustColumn( 0 );
        tab_.adjustColumn( 1 );
        // set table item properties
        pItemInt->GetValidator().setBottom( 0 );
        // connect items & datas
        pItemCombo->GetConnector().Connect( &static_cast< ADN_People_Data::PeopleInfosConsumption* >( obj )->ptrResource_.GetData()->strName_ );
        pItemInt->GetConnector().Connect( &static_cast< ADN_People_Data::PeopleInfosConsumption* >( obj )->consumption_ );
    }
private:
    ADN_Consumptions_Table_Connector& operator=( const ADN_Consumptions_Table_Connector& );
};

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table constructor
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Consumptions_Table::ADN_Consumptions_Table( QWidget* parent /* = 0*/ )
    : ADN_Table2( parent, "ADN_Consumptions_Table" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::Single );
    setShowGrid( false );
    setLeftMargin( 0 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "Resource network" ) );
    horizontalHeader()->setLabel( 1, tr( "Consumption" ) );
    // connector creation
    pConnector_ = new ADN_Consumptions_Table_Connector( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table destructor
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Consumptions_Table::~ADN_Consumptions_Table()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Consumptions_Table::OnContextMenu
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Consumptions_Table::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    std::auto_ptr< Q3PopupMenu > pTargetMenu( new Q3PopupMenu( this ) );
    // Get the list of the possible munitions
    bool bDisplayAdd = false;
    bool bDisplayRem = GetCurrentData() != 0;
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
    ADN_People_Data::PeopleInfosConsumption* pCurResource = static_cast< ADN_People_Data::PeopleInfosConsumption* >( GetCurrentData() );
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
    while( ADN_TableItem_ABC* pItem = static_cast< ADN_TableItem_ABC* >( item( n, 1 ) ) )
    {
        ADN_People_Data::PeopleInfosConsumption* pInfos = static_cast< ADN_People_Data::PeopleInfosConsumption* >( pItem->GetData() );
        if( pInfos->ptrResource_.GetData() == &infos )
            return true;
        ++n;
    }
    return false;
}
