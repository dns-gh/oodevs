#include "adaptation_app_pch.h"
#include "ADN_Composantes_Speeds_GUI.h"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Composantes_Data.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"

typedef ADN_Composantes_Data::SpeedInfos SpeedInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Composantes_Speeds_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Composantes_Speeds
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Composantes_Speeds(ADN_Composantes_Speeds_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {

        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Double *pItemSpeed=0;

        // add a new row & set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemSpeed=new ADN_TableItem_Double(&tab_,obj));

        // set table item properties
        pItemString->setEnabled(false);
        pItemString->setText(ADN_Tr::ConvertFromLocation(static_cast<SpeedInfos*>(obj)->nTypeTerrain_,ENT_Tr_ABC::eToTr).c_str());

        // set table item properties
        pItemSpeed->GetValidator().setBottom(0);

        // connect items & datas
        pItemSpeed->GetConnector().Connect(&static_cast<SpeedInfos*>(obj)->rSpeed_);
    }

private:
    ADN_CT_Composantes_Speeds& operator=( const ADN_CT_Composantes_Speeds& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Speeds_GUI::ADN_Composantes_Speeds_GUI(QWidget * parent )
:   ADN_Table2( parent, "ADN_Composantes_Speeds_GUI" )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(Q3Table::Single);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols( 2 );
    setNumRows( 0 );
//    setColumnStretchable( 0, true );
//    setColumnStretchable( 1, false );

    horizontalHeader()->setLabel( 0, tr( "Ground type") );
    horizontalHeader()->setLabel( 1, tr( "Speed (km/h)") );

    // connector creation
    pConnector_=new ADN_CT_Composantes_Speeds(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Speeds_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_Speeds_GUI::~ADN_Composantes_Speeds_GUI()
{
    delete pConnector_;
}
