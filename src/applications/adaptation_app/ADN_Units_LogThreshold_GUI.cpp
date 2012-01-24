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
class ADN_Units_LogThreshold_GUI_Connector
:public ADN_Connector_Table_ABC
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

        pItemName->setEnabled(false);
        pItemName->setText( ENT_Tr::ConvertFromStockCategory( pInfo->eCategory_.GetData(), ENT_Tr::eToApp ).c_str() );

        // Connect the item
        pItemLogThreshold->GetConnector().Connect( &pInfo->rLogThreshold_ );
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

    horizontalHeader()->setLabel( 0, tr( "Category" ) );
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

    for( uint i = 0; i < eNbrStockCategory; ++i )
    {
        // This id is used to encode the category into the item.
        int nItemId = i;
        if( !Contains( (E_StockCategory)i ) )
            targetMenu.insertItem( ENT_Tr::ConvertFromStockCategory( (E_StockCategory)i ).c_str(), nItemId );
    }

    if( !targetMenu.isEmpty() )
        menu.insertItem( tr( "Add class"), &targetMenu , 667 );
    if( GetCurrentData() != 0 )
        menu.insertItem( tr( "Remove class" ), 666 );

    int nMenuResult = menu.exec(pt);
    if( nMenuResult >= 0 )
    {
        if( nMenuResult == 666 )
            RemoveCurrentLogSupplyClass();
        else if( nMenuResult != 667 )
            AddNewLogSupplyClass( (E_StockCategory)nMenuResult );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass
// Created: AGN 2003-12-04
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::AddNewLogSupplyClass( E_StockCategory category )
{
    ADN_Units_Data::StockLogThresholdInfos* pNewInfo = new ADN_Units_Data::StockLogThresholdInfos();
    pNewInfo->eCategory_ = category;
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

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::Contains
// Created: AGN 2004-02-25
// -----------------------------------------------------------------------------
bool ADN_Units_LogThreshold_GUI::Contains( E_StockCategory category )
{
    int n = 0;
    while( item( n, 1 ) != 0 )
    {
        ADN_TableItem_ABC* pItem = static_cast<ADN_TableItem_ABC*>( item( n, 1 ) );
        ADN_Units_Data::StockLogThresholdInfos* pInfos = static_cast<ADN_Units_Data::StockLogThresholdInfos*>( pItem->GetData() );
        if( pInfos->eCategory_ == category )
            return true;
        ++n;
    }
    return false;
}

