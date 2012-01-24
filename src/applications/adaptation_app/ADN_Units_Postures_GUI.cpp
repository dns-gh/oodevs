//*****************************************************************************
//
// $Created: JDY 03-07-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Postures_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:56 $
// $Revision: 10 $
// $Workfile: ADN_Units_Postures_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Units_Postures_GUI.h"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Units_Data.h"
#include "ENT/ENT_Tr.h"
#include "ADN_TableItem_TimeField.h"

typedef ADN_Units_Data::PostureInfos PostureInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Units_Postures_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Units_Postures
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Units_Postures(ADN_Units_Postures_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void AddSubItems( int n, void *pObj )
    {
        assert( pObj != 0 );
        PostureInfos* pInfo = (PostureInfos*)pObj;

        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_TimeField* pItemDoubleTimeToActivate = new ADN_TableItem_TimeField( &tab_, pObj );

        // Add a new row.
        tab_.setItem( n, 0, pItemString );
        tab_.setItem( n, 1, pItemDoubleTimeToActivate );

        // Set the table item properties and connect the data.
        pItemString->setEnabled( false );
        pItemString->setText( ENT_Tr::ConvertFromUnitPosture( pInfo->nPosture_, ENT_Tr_ABC::eToTr ).c_str() );

        if( pInfo->nPosture_ < ePostureNeedTimeStart )
            pItemDoubleTimeToActivate->setEnabled( false );
        else
            pItemDoubleTimeToActivate->GetConnector().Connect( &pInfo->timeToActivate_ );
    }

private:
    ADN_CT_Units_Postures& operator=( const ADN_CT_Units_Postures& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Units_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Postures_GUI::ADN_Units_Postures_GUI( QWidget* pParent )
: ADN_Table2( pParent, "ADN_Units_Postures_GUI" )
{
    // Selection and sorting.
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );

    setFixedHeight( 110 );

    // Hide the vertical header.
    verticalHeader()->hide();

    // Setup 2 columns.
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    horizontalHeader()->setLabel(0, tr( "Stance"));
    horizontalHeader()->setLabel(1, tr( "Time to activate"));

    // Create the connector.
    pConnector_ = new ADN_CT_Units_Postures(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Units_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Units_Postures_GUI::~ADN_Units_Postures_GUI()
{
    delete pConnector_;
}

