//*****************************************************************************
//
// $Created: JDY 03-09-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_ComboBoxItem.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 15:46 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_ComboBoxItem.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_TableItem_ComboBoxItem.h"

#include "ADN_Connector_String.h"

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem constructor
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBoxItem::ADN_TableItem_ComboBoxItem(ADN_TableItem_ComboBox& combo,void *data)
: combo_(combo)
, pData_(data)
, szTmpTxt_()
{
    pConnector_=new ADN_Connector_String<ADN_TableItem_ComboBoxItem>(this);
    assert(pConnector_);
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem destructor
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
ADN_TableItem_ComboBoxItem::~ADN_TableItem_ComboBoxItem()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem::setText
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBoxItem::setText(const QString& txt)
{
    // look if combo item is present in combo box
    // and find its index
    int ndx=combo_.FindNdx(this);
    if( ndx != -1 )
        combo_.changeItem( txt , ndx);
    else
        szTmpTxt_=txt;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem::text
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
QString ADN_TableItem_ComboBoxItem::text() const
{
    // look if combo item is present in combo box
    // and find its index
    int ndx=combo_.FindNdx(this);
    if( ndx != -1 )
        return combo_.text( ndx);
    else
        return szTmpTxt_;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem::setEnabled
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
void ADN_TableItem_ComboBoxItem::setEnabled(bool /*bEnable*/)
{
}