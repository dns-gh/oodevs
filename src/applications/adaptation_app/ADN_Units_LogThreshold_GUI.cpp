//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_LogThreshold_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:55 $
// $Revision: 11 $
// $Workfile: ADN_Units_LogThreshold_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Units_LogThreshold_GUI.h"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Units_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Tr.h"
#include "ENT/Ent_Tr.h"

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Units_LogThreshold_GUI_Connector : public ADN_Connector_Table_ABC
{
public:

    ADN_Units_LogThreshold_GUI_Connector( ADN_Units_LogThreshold_GUI& tab )
    : ADN_Connector_Table_ABC( tab, false )
    {}

    void  AddSubItems( int nRow, void* pObj )
    {
        assert( pObj != 0 );
        ADN_Units_Data::StockLogThresholdInfos* pInfo = static_cast< ADN_Units_Data::StockLogThresholdInfos* >( pObj );

        // Add a new row.
        ADN_TableItem_String* pItemName = new ADN_TableItem_String( &tab_, pObj, Q3TableItem::Never );
        ADN_TableItem_Double* pItemLogThreshold  = new ADN_TableItem_Double( &tab_, pObj );

        pItemLogThreshold->GetValidator().setRange( 0, 100, 2 );

        tab_.setItem( nRow, 0, pItemName );
        tab_.setItem( nRow, 1, pItemLogThreshold );

        // Connect the item
        pItemLogThreshold->GetConnector().Connect( &pInfo->rLogThreshold_ );
        pItemName->GetConnector().Connect( pInfo->ptrLogisticSupplyClass_.GetData() );
    }

private:
    ADN_Units_LogThreshold_GUI_Connector& operator=( const ADN_Units_LogThreshold_GUI_Connector& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_LogThreshold_GUI::ADN_Units_LogThreshold_GUI( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Units_LogThreshold_GUI" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );

    // Hide the vertical header.
    verticalHeader()->hide();
    setLeftMargin( 0 );

    // Setup the columns.
    setNumRows( 0 );
    setNumCols( 2 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    horizontalHeader()->setLabel( 0, tr( "Logistic supply class" ) );
    horizontalHeader()->setLabel( 1, tr( "Log threshold (%)" ) );

    pConnector_ = new ADN_Units_LogThreshold_GUI_Connector( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_LogThreshold_GUI::~ADN_Units_LogThreshold_GUI()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::OnContextMenu
// Created: AGN 03-08-04
//-----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::OnContextMenu( int /*row*/, int /*col*/, const QPoint& pt )
{
    Q3PopupMenu menu( this );
    Q3PopupMenu targetMenu( &menu );

    // Get the list.
    helpers::T_LogisticSupplyClass_Vector& logisticSupplyClasses = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetLogisticSupplyClasses();

    // Fill the popup menu with submenus, one for each dotation.
    for( helpers::IT_LogisticSupplyClass_Vector it = logisticSupplyClasses.begin(); it != logisticSupplyClasses.end(); ++it )
    {
        // This id is used to encode the category into the item.
        int nItemId = (int)(*it);
        targetMenu.insertItem( (*it)->GetData().c_str(), nItemId );
    }

    menu.insertItem( tr( "Add class"), &targetMenu ,0 );
    if( GetCurrentData() != 0 )
        menu.insertItem( tr( "Remove class" ), 1 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult == 1 )
        RemoveCurrentLogSupplyClass();
    else if( nMenuResult > 1 )
        AddNewLogSupplyClass( *(helpers::LogisticSupplyClass*)nMenuResult );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass( helpers::LogisticSupplyClass& category )
{
    ADN_Units_Data::StockLogThresholdInfos* pNewInfo = new ADN_Units_Data::StockLogThresholdInfos();
    pNewInfo->ptrLogisticSupplyClass_ = &category;
    pNewInfo->rLogThreshold_ = 10;

    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    pCTable->AddItem( pNewInfo );
    pCTable->AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::RemoveCurrentLogSupplyClass
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::RemoveCurrentLogSupplyClass()
{
    assert( GetCurrentData() != 0 );
    static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( GetCurrentData() );
}

