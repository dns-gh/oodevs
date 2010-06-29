// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ComboBox_ActiveProtection_Dotations.h"

#include "ADN_Equipement_Data.h"

typedef ADN_Equipement_Data::DotationInfos DotationInfos;

class ADN_CBB_ActiveProtection_Dotations
: public ADN_Connector_ComboBox
{
public:
    ADN_CBB_ActiveProtection_Dotations( ADN_ComboBox_ActiveProtection_Dotations& combo )
    :   ADN_Connector_ComboBox(&combo)
    {}

    virtual ~ADN_CBB_ActiveProtection_Dotations()
    {}

    ADN_ComboBoxItem* CreateItem(void * obj)
    {
        // create new combo item
        ADN_ComboBoxItem* pItem = new ADN_ComboBoxItem(*pCombo_,obj);

        // connect it with armor name
        pItem->GetConnector().Connect( (DotationInfos*)obj );

        // return
        return pItem;
    }
};

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_ActiveProtection_Dotations constructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ComboBox_ActiveProtection_Dotations::ADN_ComboBox_ActiveProtection_Dotations(QWidget * parent, const char * name)
: ADN_ComboBox(parent,name)
{
    // connector creation
    pConnector_=new ADN_CBB_ActiveProtection_Dotations(*this);
}

//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_ActiveProtection_Dotations destructor
// Created: FDS 2010-02-24
//-----------------------------------------------------------------------------
ADN_ComboBox_ActiveProtection_Dotations::~ADN_ComboBox_ActiveProtection_Dotations()
{
    delete pConnector_;
}
