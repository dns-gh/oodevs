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

// -----------------------------------------------------------------------------
// Name: ADN_Units_LogThreshold_GUI::InitializeLogThresholds
// Created: SBO 2006-01-11
// -----------------------------------------------------------------------------
void ADN_Units_LogThreshold_GUI::InitializeLogThresholds()
{
    ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
    for( uint i = 0; i < eNbrStockCategory; ++i )
    {
        std::auto_ptr< ADN_Units_Data::StockLogThresholdInfos > spNew( new ADN_Units_Data::StockLogThresholdInfos( ( E_StockCategory )i ) );
        //vLogThresholds_.AddItem( spNew.release() );
        pCTable->AddItem( spNew.release() );
    }
    pCTable->AddItem( 0 );
}
