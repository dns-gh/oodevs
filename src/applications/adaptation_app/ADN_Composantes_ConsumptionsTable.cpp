// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-25 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Composantes_ConsumptionsTable.cpp $
// $Author: Nld $
// $Modtime: 28/04/05 15:03 $
// $Revision: 8 $
// $Workfile: ADN_Composantes_ConsumptionsTable.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composantes_ConsumptionsTable.h"
#include "moc_ADN_Composantes_ConsumptionsTable.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Composantes_Data.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Validator.h"
#include "ADN_Tr.h"
#include "ADN_ListView_Composantes.h"


typedef ADN_Composantes_Data::ConsumptionItem ConsumptionItem;


//-----------------------------------------------------------------------------
// Internal Table connector
//-----------------------------------------------------------------------------
class ADN_Connector_ConsumptionTable
    : public ADN_Connector_Table_ABC
{
public:
    ADN_Connector_ConsumptionTable( ADN_Composantes_ConsumptionsTable& tab )
        : ADN_Connector_Table_ABC( tab, true, "ADN_Connector_ConsumptionTable" )
    {}

    void AddSubItems( int n, void* pObj )
    {
        assert( pObj != 0 );
        ConsumptionItem* pConsumption = static_cast<ConsumptionItem*>( pObj );

        ADN_TableItem_String* pItemType     = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_String* pItemCategory = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_Double* pItemQtyUsed  = new ADN_TableItem_Double( &tab_, pObj );

        // Add a new row.
        tab_.setItem( n, 0, pItemType );
        tab_.setItem( n, 1, pItemCategory );
        tab_.setItem( n, 2, pItemQtyUsed );

        // Set the item properties.
        pItemType->setEnabled( false );
        pItemType->setText( ADN_Tr::ConvertFromConsumptionType( pConsumption->nConsumptionType_, ADN_Tr::eToTr ).c_str() );
        pItemCategory->setEnabled( false );
        pItemCategory->SetAutoEnabled( false );
        pItemCategory->setText( pConsumption->ptrCategory_.GetData()->strName_.GetData().c_str() );
        pItemQtyUsed->GetValidator().setBottom( 0 );

        // Connect the items.
        pItemCategory->GetConnector().Connect( &pConsumption->ptrCategory_.GetData()->strName_ );
        pItemQtyUsed->GetConnector().Connect( &pConsumption->nQuantityUsedPerHour_ );
    }

    bool LessComparison( void* pL, void* pR ) const
    {
        ConsumptionItem* pLInfo = static_cast<ConsumptionItem*>( pL );
        ConsumptionItem* pRInfo = static_cast<ConsumptionItem*>( pR );
        return pLInfo->nConsumptionType_ < pRInfo->nConsumptionType_;
    }
};


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Composantes_ConsumptionsTable::ADN_Composantes_ConsumptionsTable(  ADN_ListView_Composantes& composanteListView, QWidget* pParent )
: ADN_Table2         ( pParent, "ADN_Composantes_ConsumptionsTable" )
, composanteListView_( composanteListView )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    setMinimumHeight( 115 );
    setMaximumHeight( 115 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 3 columns.
    setNumCols( 3 );
    setNumRows( 0 );
//    setColumnStretchable( 0, true );
//    setColumnStretchable( 1, true );
//    setColumnStretchable( 2, false );

    horizontalHeader()->setLabel( 0, tr( "Activity" ) );
    horizontalHeader()->setLabel( 1, tr( "Consumed item" ) );
    horizontalHeader()->setLabel( 2, tr( "Qty/hour" ) );

    // Create the connector.
    pConnector_ = new ADN_Connector_ConsumptionTable( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable destructor
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
ADN_Composantes_ConsumptionsTable::~ADN_Composantes_ConsumptionsTable()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::OnContextMenu
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::OnContextMenu( int /*nRow*/, int /*nCol*/, const QPoint& pt )
{
    /**********************************************/
    menuParametersList_.clear();

    // Get the available category list.
    assert( composanteListView_.GetCurrentData() != 0 );
    ADN_Composantes_Data::ComposanteInfos* pComp = (ADN_Composantes_Data::ComposanteInfos*)(composanteListView_.GetCurrentData());
    ADN_Composantes_Data::ResourceInfos& dotation = pComp->resources_;
    ADN_Composantes_Data::T_CategoryInfos_Vector& categories = dotation.categories_;

    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );

    int nItemId = 0;
    // Iterate over the consumption types, and create an 'add' submenu for each of them
    for( int nConsumption = 0; nConsumption < eNbrConsumptionType; ++nConsumption )
    {
        Q3PopupMenu* pConsumptionMenu = new Q3PopupMenu( &addMenu );
        addMenu.insertItem( ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nConsumption ).c_str(), pConsumptionMenu );

        // Fill the popup menu with submenus, one for each dotation.
        for( ADN_Composantes_Data::IT_CategoryInfos_Vector it = categories.begin(); it != categories.end(); ++it )
        {
            // Add the item.
            pConsumptionMenu->insertItem( (*it)->ptrCategory_.GetData()->strName_.GetData().c_str(), nItemId );

            //Save its parameters.
            menuParametersList_.push_back( T_MenuItemParameters( (E_ConsumptionType)nConsumption, (*it)->ptrCategory_.GetData() ) );
            nItemId++;

            ADN_Tools::SortMenu( *pConsumptionMenu );
        }
    }

    menu.insertItem( tr( "Add consumption"), &addMenu );
    if( GetCurrentData() != 0 )
        menu.insertItem( tr( "Remove consumption" ), 9999999 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 9999999 )
    {
        DeleteCurrentElement();
    }
    else if( nMenuResult >= 0 )
    {
        T_MenuItemParameters parameters = menuParametersList_[nMenuResult];
        CreateNewElement( parameters );
    }
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::CreateNewElement
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::CreateNewElement( T_MenuItemParameters& parameters )
{
    ConsumptionItem* pNewItem = new ConsumptionItem( parameters.first, * parameters.second );
    pNewItem->nQuantityUsedPerHour_ = 0;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewItem );
    pCTable->AddItem( 0 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::DeleteCurrentElement
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::DeleteCurrentElement()
{
    assert( GetCurrentData() != 0 );

    // Remove the current data from the list.
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetCurrentData() );
}
