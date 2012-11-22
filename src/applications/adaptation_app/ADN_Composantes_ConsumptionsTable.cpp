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
#include "ADN_Validator.h"
#include "ADN_Tr.h"
#include "ADN_ListView_Composantes.h"

typedef ADN_Composantes_Data::ConsumptionItem ConsumptionItem;

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable constructor
// Created: APE 2005-01-07
// -----------------------------------------------------------------------------
ADN_Composantes_ConsumptionsTable::ADN_Composantes_ConsumptionsTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent /*= 0*/ )
    : ADN_Table( objectName, connector, pParent )
    , composanteListView_( 0 )
{
    dataModel_.setColumnCount( 3 );
    QStringList horizontalHeaders;
    horizontalHeaders << tr( "Activity" )
                      << tr( "Consumed item" )
                      << tr( "Qty/hour" );
    dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
    horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 1, QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    verticalHeader()->setVisible( false );
    delegate_.AddDoubleSpinBoxOnColumn( 2, 0, INT_MAX );
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    setShowGrid( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable destructor
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
ADN_Composantes_ConsumptionsTable::~ADN_Composantes_ConsumptionsTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::SetListView
// Created: ABR 2012-01-20
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::SetListView( ADN_ListView* listView )
{
    composanteListView_ = listView;
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::OnContextMenu
// Created: APE 2005-01-25
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::OnContextMenu( const QPoint& pt )
{
    /**********************************************/
    menuParametersList_.clear();

    // Get the available category list.
    assert( composanteListView_ != 0 && composanteListView_->GetCurrentData() != 0 );
    ADN_Composantes_Data::ComposanteInfos* pComp = (ADN_Composantes_Data::ComposanteInfos*)( composanteListView_->GetCurrentData() );
    ADN_Composantes_Data::ResourceInfos& dotation = pComp->resources_;
    ADN_Composantes_Data::T_CategoryInfos_Vector& categories = dotation.categories_;

    Q3PopupMenu menu( this );
    Q3PopupMenu addMenu( &menu );

    int nItemId = 0;
    // Iterate over the consumption types, and create an 'add' submenu for each of them
    for( int nConsumption = 0; nConsumption < eNbrConsumptionType; ++nConsumption )
    {
        const QString consumptionName = ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)nConsumption, ADN_Tr::eToTr ).c_str();
        int count = 0;
        for( int i = 0; i < numRows(); ++i )
            if( dataModel_.item( i, 0 ) && dataModel_.item( i, 0 )->text() == consumptionName )
                ++count;
        if( count == categories.size() )
            continue;

        Q3PopupMenu* pConsumptionMenu = new Q3PopupMenu( &addMenu );
        addMenu.insertItem( consumptionName, pConsumptionMenu );

        // Fill the popup menu with submenus, one for each dotation.
        for( ADN_Composantes_Data::IT_CategoryInfos_Vector it = categories.begin(); it != categories.end(); ++it )
        {
            bool alreadyAdded = false;
            for( int i = 0; i < numRows() && !alreadyAdded; ++i )
                if( dataModel_.item( i, 0 ) && dataModel_.item( i, 0 )->text() == consumptionName &&
                    dataModel_.item( i, 1 ) && dataModel_.item( i, 1 )->text() == (*it)->ptrCategory_.GetData()->strName_.GetData().c_str() )
                    alreadyAdded = true;
            if( alreadyAdded )
                continue;

            // Add the item.
            pConsumptionMenu->insertItem( (*it)->ptrCategory_.GetData()->strName_.GetData().c_str(), nItemId++ );
            //Save its parameters.
            menuParametersList_.push_back( T_MenuItemParameters( (E_ConsumptionType)nConsumption, (*it)->ptrCategory_.GetData() ) );

            ADN_Tools::SortMenu( *pConsumptionMenu );
        }
    }

    menu.insertItem( tr( "Add consumption"), &addMenu );
    if( GetSelectedData() != 0 )
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
    assert( GetSelectedData() != 0 );

    // Remove the current data from the list.
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetSelectedData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Composantes_ConsumptionsTable::AddRow
// Created: MMC 2012-11-06
// -----------------------------------------------------------------------------
void ADN_Composantes_ConsumptionsTable::AddRow( int row, void* data )
{
    ConsumptionItem* pConsumption = static_cast<ConsumptionItem*>( data );
    if( !pConsumption )
        return;
    AddItem( row, 0, data, QString::fromStdString(  ADN_Tr::ConvertFromConsumptionType( pConsumption->nConsumptionType_, ADN_Tr::eToTr ).c_str() )
                         , Qt::ItemIsSelectable );
    AddItem( row, 1, data, &pConsumption->ptrCategory_.GetData()->strName_, ADN_StandardItem::eString, Qt::ItemIsSelectable );
    AddItem( row, 2, data, &pConsumption->nQuantityUsedPerHour_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
}
