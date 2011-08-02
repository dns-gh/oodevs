//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Sizes_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:53 $
// $Revision: 8 $
// $Workfile: ADN_Sensors_Sizes_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_Sizes_GUI.h"
#include "moc_ADN_Sensors_Sizes_GUI.cpp"

#include "ADN_App.h"
#include "ADN_CommonGfx.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Sensors_Data::ModificatorSizeInfos       ModificatorSizeInfos;


//-----------------------------------------------------------------------------
// Internal Table connector to be connected with ADN_Sensors_Sizes_GUI
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_Sizes
:public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Sensors_Sizes(ADN_Sensors_Sizes_GUI& tab)
    : ADN_Connector_Table_ABC(tab,false)
    {}

    void  AddSubItems(int i,void *obj)
    {
        assert(obj);
        ADN_TableItem_String *pItemString=0;
        ADN_TableItem_Double *pItemDouble=0;

        // add a new row & set new values
        tab_.setItem(i,0,pItemString=new ADN_TableItem_String(&tab_,obj));
        tab_.setItem(i,1,pItemDouble=new ADN_TableItem_Double(&tab_,obj));

        // disable composante category name
        pItemString->setEnabled(false);
        pItemString->SetAutoEnabled(false);

        // set table item properties
        pItemDouble->GetValidator().setRange( 0, 1, 2 );

        // connect items & datas
        pItemString->GetConnector().Connect(static_cast<ModificatorSizeInfos*>(obj)->ptrSize_.GetData());
        pItemDouble->GetConnector().Connect(&static_cast<ModificatorSizeInfos*>(obj)->rCoeff_);
    }


private:
    ADN_CT_Sensors_Sizes& operator=( const ADN_CT_Sensors_Sizes& );
};



//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Sizes_GUI::ADN_Sensors_Sizes_GUI(QWidget * parent )
:   ADN_Table2(parent, "ADN_Sensors_Sizes_GUI" )
{
    // peut etre selectionne & trie
    setSorting(true);
    setSelectionMode(Q3Table::NoSelection);
    setShowGrid(false);
    setLeftMargin(0);

    // hide vertical header
    verticalHeader()->hide();

    // tab with 2 columns
    setNumCols(2);
    setNumRows(0);
    setColumnStretchable(0,true);
    setColumnStretchable(1,true);
    setMaximumWidth( 300 );

    horizontalHeader()->setLabel(0, tr( "Volumes" ) );
    horizontalHeader()->setLabel(1, tr( "Modifiers" ) );

    // connector creation
    pConnector_=new ADN_CT_Sensors_Sizes(*this);
    connect( this, SIGNAL( currentChanged( int, int ) ), SLOT( OnCurrentChanged() ) );
    connect( this, SIGNAL( selectionChanged() ), SLOT( OnCurrentChanged() ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Sizes_GUI::~ADN_Sensors_Sizes_GUI()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Sizes_GUI::OnCurrentChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_Sizes_GUI::OnCurrentChanged()
{
     ModificatorSizeInfos* data = static_cast< ModificatorSizeInfos* >( GetCurrentData() );
     if( data  && data->ptrSize_.GetData() )
        emit SizeChanged( data->GetItemName(), data->rCoeff_.GetData() );
}
