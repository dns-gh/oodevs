//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_TableItem_ABC.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:38 $
// $Revision: 5 $
// $Workfile: ADN_TableItem_ABC.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_TableItem_ABC.h"


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_TableItem_ABC::ADN_TableItem_ABC(ADN_Table*parent,void*data, QTableItem::EditType nEditType)
: QTableItem(parent,nEditType,"")
, QObject(parent)
, ADN_Gfx_ABC()
, pData_(data)
{
    setReplaceable( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_TableItem_ABC::~ADN_TableItem_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC::key
// Created: APE 2005-04-12
// -----------------------------------------------------------------------------
QString ADN_TableItem_ABC::key() const
{
    QString strText = this->text();
    QString strKey;
    strKey.fill( ' ', 30 - strText.length() );
    strKey.append( strText );
    return strKey;
}
