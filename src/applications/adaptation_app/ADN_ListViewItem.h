//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListViewItem.h $
// $Author: Ape $
// $Modtime: 28/02/05 11:14 $
// $Revision: 3 $
// $Workfile: ADN_ListViewItem.h $
//
//*****************************************************************************

#ifndef __ADN_ListViewItem_h_
#define __ADN_ListViewItem_h_

class ADN_ListView;

#include "ADN_Connector_ABC.h"

//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
class ADN_ListViewItem
:   public Q3ListViewItem
{
public:

    enum E_TypeCvt
    {
        eString,
        eInt,
        eBool,
        eDouble
    };

public:

    explicit ADN_ListViewItem(ADN_ListView* parent,void* data,int ncol);
    virtual ~ADN_ListViewItem();

    void *  GetData();
    void    Connect(int ndx,ADN_Connector_ABC *item,E_TypeCvt cvt=eString);

protected:

    virtual void okRename ( int col );

private:

    void*                       pData_;
    T_ConnectorVector        vConnectors_;
};


//-----------------------------------------------------------------------------
// Name: ADN_ListViewItem::GetItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
inline
void* ADN_ListViewItem::GetData()
{
    return pData_;
}


#endif // __ADN_ListViewItem_h_