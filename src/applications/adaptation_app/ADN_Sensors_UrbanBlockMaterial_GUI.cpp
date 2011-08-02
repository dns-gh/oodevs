//*****************************************************************************
//
// $Created: SLG 2010-03-02
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_UrbanBlockMaterial_GUI.cpp $
// $Author: Ape $
// $Modtime: 20/04/05 16:50 $
// $Revision: 9 $
// $Workfile: ADN_Sensors_UrbanBlockMaterial_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Sensors_UrbanBlockMaterial_GUI.h"
#include "moc_ADN_Sensors_UrbanBlockMaterial_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Tools.h"
#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Tr.h"

typedef ADN_Sensors_Data::SensorInfos                   SensorInfos;
typedef ADN_Sensors_Data::ModificatorUrbanBlockInfos    ModificatorUrbanBlockInfos;

//-----------------------------------------------------------------------------
// Internal Table connector to be connected with T_ModificatorEnvironmentInfos_Vector
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_UrbanBlock
    :public ADN_Connector_Table_ABC
{
public:

    ADN_CT_Sensors_UrbanBlock(ADN_Sensors_UrbanBlockMaterial_GUI& tab)
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

        // disable first column
        pItemString->setEnabled(false);


        // set table item properties
        pItemDouble->GetValidator().setRange( 0, 1, 2 );

        // connect items & datas
        pItemString->GetConnector().Connect(&static_cast<ModificatorUrbanBlockInfos*>(obj)->ptrMaterial_.GetData()->strName_);
        pItemDouble->GetConnector().Connect(&static_cast<ModificatorUrbanBlockInfos*>(obj)->rCoeff_);
    }

private:
    ADN_CT_Sensors_UrbanBlock& operator=( const ADN_CT_Sensors_UrbanBlock& );
};


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI constructor
// Created: SLG 2010-03-02
//-----------------------------------------------------------------------------
ADN_Sensors_UrbanBlockMaterial_GUI::ADN_Sensors_UrbanBlockMaterial_GUI(QWidget * parent)
:   ADN_Table2(parent, "ADN_Sensors_UrbanBlockMaterial_GUI" )
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

    horizontalHeader()->setLabel(0, tr( "Material" ) );
    horizontalHeader()->setLabel(1, tr( "Modifiers" ) );

    // connector creation
    pConnector_=new ADN_CT_Sensors_UrbanBlock(*this);
    connect( this, SIGNAL( currentChanged( int, int ) ), SLOT( OnCurrentChanged() ) );
    connect( this, SIGNAL( selectionChanged() ), SLOT( OnCurrentChanged() ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI destructor
// Created: SLG 2010-03-02
//-----------------------------------------------------------------------------
ADN_Sensors_UrbanBlockMaterial_GUI::~ADN_Sensors_UrbanBlockMaterial_GUI()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_UrbanBlockMaterial_GUI::OnCurrentChanged
// Created: HBD 2010-05-03
// -----------------------------------------------------------------------------
void ADN_Sensors_UrbanBlockMaterial_GUI::OnCurrentChanged()
{
    if( ModificatorUrbanBlockInfos* data = static_cast< ModificatorUrbanBlockInfos* >( GetCurrentData() ) )
        if( data  && data->ptrMaterial_.GetData() )
            emit UrbanBlockChanged( data->GetItemName(), data->rCoeff_.GetData() );

}
