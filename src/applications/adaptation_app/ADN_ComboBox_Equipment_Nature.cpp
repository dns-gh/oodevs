//*****************************************************************************
//
// $Created: JDY 03-09-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox_Composantes_Sizes.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 15:46 $
// $Revision: 2 $
// $Workfile: ADN_ComboBox_Composantes_Sizes.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ComboBox_Equipment_Nature.h"

#include "ADN_Categories_Data.h"
#include "ADN_ResourceNatureInfos.h"

class ADN_CCB_Equipement_Nature
: public ADN_Connector_ComboBox
{
public:
    ADN_CCB_Equipement_Nature( ADN_ComboBox_Equipment_Nature& combo )
    :   ADN_Connector_ComboBox(&combo)
    {}

    virtual ~ADN_CCB_Equipement_Nature()
    {}

    ADN_ComboBoxItem* CreateItem(void * obj)
    {
        // create new combo item
        ADN_ComboBoxItem* pItem = new ADN_ComboBoxItem(*pCombo_,obj);

        // connect it with armor name
        pItem->GetConnector().Connect( (helpers::ResourceNatureInfos*)obj );

        // return
        return pItem;
    }

};


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Equipment_Nature constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox_Equipment_Nature::ADN_ComboBox_Equipment_Nature(QWidget * parent, const char * name)
: ADN_ComboBox(parent,name)
{
    // connector creation
    pConnector_=new ADN_CCB_Equipement_Nature(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Equipment_Nature destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox_Equipment_Nature::~ADN_ComboBox_Equipment_Nature()
{
    delete pConnector_;
}

