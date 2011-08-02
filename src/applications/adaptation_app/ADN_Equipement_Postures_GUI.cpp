//*****************************************************************************
//
// $Created: JDY 03-09-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Postures_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 17:04 $
// $Revision: 9 $
// $Workfile: ADN_Equipement_Postures_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_Postures_GUI.h"
#include <Qt3Support/q3popupmenu.h>
#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Workspace.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Equipement_Data::ModificatorPostureInfos ModificatorPostureInfos;

//-----------------------------------------------------------------------------
// Internal Table connector for ADN_Equipement_Postures_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Equipement_Postures : public ADN_Connector_Table_ABC
{

public:
    explicit ADN_CT_Equipement_Postures( ADN_Equipement_Postures_GUI& tab )
        : ADN_Connector_Table_ABC( tab, false )
    {}

    void AddSubItems( int i, void* pObj )
    {
        assert( pObj != 0 );
        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, pObj );
        ADN_TableItem_Double* pItemDouble = new ADN_TableItem_Double( &tab_, pObj );

        // Add a new row & set the new values.
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemDouble );

        // Setup the row header item.
        pItemString->setEnabled( false );
        pItemString->setText( ENT_Tr::ConvertFromUnitPosture( static_cast<ModificatorPostureInfos*>(pObj)->eType_, ENT_Tr_ABC::eToTr ).c_str() );

        // Setup the value item.
        pItemDouble->GetValidator().setRange( 0, 1, 2 );
        pItemDouble->GetConnector().Connect( &static_cast<ModificatorPostureInfos*>(pObj)->rCoeff_ );
    }
};


//-----------------------------------------------------------------------------
// Name: ADN_Equipement_Postures_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_Postures_GUI::ADN_Equipement_Postures_GUI( const QString& strColCaption, QWidget* pParent )
    : ADN_Table2( pParent, "ADN_Equipement_Postures_GUI" )
{
    // Setup the table properties.
    setSelectionMode( Q3Table::NoSelection );
    setSorting( true );
    setShowGrid( false );
    setLeftMargin( 0 );
    setMinimumHeight( int( ( eNbrUnitPosture + 1.2 ) * 20 ) );

    // Setup the columns and headers
    setNumCols( 2 );
    setNumRows( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );

    verticalHeader()->hide();
    horizontalHeader()->setLabel( 0, strColCaption );
    horizontalHeader()->setLabel( 1, tr( "PH factor" ) );

    // Create the connector.
    pConnector_ = new ADN_CT_Equipement_Postures( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_Equipement_Postures_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipement_Postures_GUI::~ADN_Equipement_Postures_GUI()
{
    delete pConnector_;
}