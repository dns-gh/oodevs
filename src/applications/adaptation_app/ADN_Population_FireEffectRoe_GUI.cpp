//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_FireEffectRoe_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 9 $
// $Workfile: ADN_Population_FireEffectRoe_GUI.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_FireEffectRoe_GUI.h"
#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Population_Data.h"
#include "ENT/ENT_Tr.h"

typedef ADN_Population_Data::FireEffectRoeInfos FireEffectRoeInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_FireEffectRoeInfos_Vector
//-----------------------------------------------------------------------------
class ADN_CT_Population_FireEffectRoe : public ADN_Connector_Table_ABC
{
public:
    ADN_CT_Population_FireEffectRoe( ADN_Population_FireEffectRoe_GUI& tab )
    : ADN_Connector_Table_ABC( tab, false )
    {
        // NOTHING
    }

    void AddSubItems( int i, void* obj )
    {
        assert( obj );
        E_PopulationRoe nRoe = static_cast< FireEffectRoeInfos* >(obj)->nRoe_;
        ADN_TableItem_String* pItemString = new ADN_TableItem_String( &tab_, obj );
        ADN_TableItem_Double* pItemSurface = new ADN_TableItem_Double( &tab_, obj );
        ADN_TableItem_Double* pItemPH = new ADN_TableItem_Double( &tab_, obj );
        // add a new row & set new values
        tab_.setItem( i, 0, pItemString );
        tab_.setItem( i, 1, pItemSurface );
        tab_.setItem( i, 2, pItemPH );
        // disable first column
        pItemString->setEnabled( false );
        pItemString->setText( ENT_Tr::ConvertFromPopulationRoe( nRoe, ENT_Tr_ABC::eToTr ).c_str() );
        // set table item properties
        pItemPH->GetValidator().setRange( 0, 1, 4 );
        // connect items & datas
        pItemSurface->GetConnector().Connect( &static_cast<FireEffectRoeInfos*>(obj)->rAttritionSurface_ );
        pItemPH->GetConnector().Connect( &static_cast<FireEffectRoeInfos*>(obj)->rPH_ );
    }

private:
    ADN_CT_Population_FireEffectRoe& operator=( const ADN_CT_Population_FireEffectRoe& );
};

//-----------------------------------------------------------------------------
// Name: ADN_Population_FireEffectRoe_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Population_FireEffectRoe_GUI::ADN_Population_FireEffectRoe_GUI( QWidget * parent )
    : ADN_Table2( parent, "ADN_Population_FireEffectRoe_GUI" )
{
    // peut etre selectionne & trie
    setSorting( true );
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );
    setLeftMargin( 0 );
    // hide vertical header
    verticalHeader()->hide();
    // tab with 3 columns
    setNumCols( 3 );
    setNumRows( 0 );
    horizontalHeader()->setLabel( 0, tr( "ROE" ) );
    horizontalHeader()->setLabel( 1, tr( "Attrition Surface (m²)" ) );
    horizontalHeader()->setLabel( 2, tr( "PH" ) );
    // connector creation
    pConnector_=new ADN_CT_Population_FireEffectRoe( *this );
}

//-----------------------------------------------------------------------------
// Name: ADN_Population_FireEffectRoe_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Population_FireEffectRoe_GUI::~ADN_Population_FireEffectRoe_GUI()
{
    delete pConnector_;
}
