//*****************************************************************************
//
// $Created: JDY 03-09-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_ModifPhs_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 8 $
// $Workfile: ADN_Launchers_ModifPhs_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Launchers_ModifPhs_GUI.h"

#include "ADN_Tools.h"
#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Weapons_GUI.h"
#include <Qt3Support/q3popupmenu.h>
#include "ENT/ENT_Tr.h"

typedef ADN_Launchers_Data::ModifPhsInfos ModifPhsInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with
//-----------------------------------------------------------------------------
class ADN_Launchers_ModifPhs_GUI_Connector
:public ADN_Connector_Table_ABC
{
public:

    ADN_Launchers_ModifPhs_GUI_Connector(ADN_Launchers_ModifPhs_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_Double              *pItemDouble=0;
        ADN_Type_Line_ABC<ModifPhsInfos>  *pLine=(ADN_Type_Line_ABC<ModifPhsInfos>*)obj;
        for ( int j=0;j<eNbrUnitPosture;++j)
        {
            // add a new row & set new values
            pItemDouble = new ADN_TableItem_Double(&tab_,obj);
            pItemDouble->UseColor( true );
            pItemDouble->SetRangeForColor( 0.0, 100.0 );
            tab_.setItem(i,j, pItemDouble );
            pItemDouble->GetValidator().setRange( 0, 100, 5 );
            pItemDouble->GetConnector().Connect(pLine->operator[](j));
        }
    }

private:
    ADN_Launchers_ModifPhs_GUI_Connector& operator=( const ADN_Launchers_ModifPhs_GUI_Connector& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Launchers_ModifPhs_GUI::ADN_Launchers_ModifPhs_GUI( QWidget * parent )
:   ADN_Table2( parent, "ADN_Launchers_ModifPhs_GUI" )
{
    // peut etre selectionne & trie
    setSelectionMode( Q3Table::NoSelection );
    setShowGrid( false );

    // columns
    setNumCols( eNbrUnitPosture );
    setNumRows( eNbrUnitPosture );
    for( int i=0; i < eNbrUnitPosture; ++i )
    {
        setColumnStretchable(i,true);
        horizontalHeader()->setLabel(i,ENT_Tr::ConvertFromUnitPosture((E_UnitPosture)i,ENT_Tr_ABC::eToTr).c_str());
        verticalHeader()->setLabel(i,ENT_Tr::ConvertFromUnitPosture((E_UnitPosture)i,ENT_Tr_ABC::eToTr).c_str());
    }

    int nRowHeight = this->rowHeight( 0 );
    this->setMaximumHeight( ( eNbrUnitPosture + 1 ) * nRowHeight + 5);

    // connector creation
    pConnector_=new ADN_Launchers_ModifPhs_GUI_Connector(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Launchers_ModifPhs_GUI::~ADN_Launchers_ModifPhs_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Launchers_ModifPhs_GUI::doValueChanged
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
void ADN_Launchers_ModifPhs_GUI::doValueChanged( int row, int col )
{
    ADN_Table2::doValueChanged( row, col );
    ADN_Workspace::GetWorkspace().GetWeapons().GetGui().UpdateModifiers();
}
