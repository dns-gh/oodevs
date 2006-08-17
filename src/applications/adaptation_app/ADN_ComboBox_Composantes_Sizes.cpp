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
#include "ADN_ComboBox_Composantes_Sizes.h"

#include "ADN_Categories_Data.h"

typedef ADN_Categories_Data::SizeInfos SizeInfos;

class ADN_CCB_Composantes_Sizes
: public ADN_Connector_ComboBox
{
public:
    ADN_CCB_Composantes_Sizes(ADN_ComboBox_Composantes_Sizes& combo)
    :   ADN_Connector_ComboBox(&combo)
    {}

    virtual ~ADN_CCB_Composantes_Sizes()
    {}

    ADN_ComboBoxItem* CreateItem(void * obj)
    {
        // create new combo item 
        ADN_ComboBoxItem* pItem=new ADN_ComboBoxItem(*pCombo_,obj);
        
        // connect it with armor name
        pItem->GetConnector().Connect( (SizeInfos*)obj );
        
        // return    
        return pItem; 
    }

};


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Composantes_Sizes constructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox_Composantes_Sizes::ADN_ComboBox_Composantes_Sizes(QWidget * parent, const char * name)
: ADN_ComboBox(parent,name)
{
    // connector creation
    pConnector_=new ADN_CCB_Composantes_Sizes(*this);
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Composantes_Sizes destructor
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
ADN_ComboBox_Composantes_Sizes::~ADN_ComboBox_Composantes_Sizes()
{
    delete pConnector_;
}

