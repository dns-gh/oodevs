//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_ABC.h $
// $Author: Ape $
// $Modtime: 12/04/05 16:51 $
// $Revision: 3 $
// $Workfile: ADN_TableItem_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_TableItem_ABC_h_
#define __ADN_TableItem_ABC_h_

#include "ADN_Table.h"

//*****************************************************************************
// Created: JDY 03-07-09
//*****************************************************************************
class ADN_TableItem_ABC
: public QTableItem
, public QObject
, public ADN_Gfx_ABC
{
public:
    explicit ADN_TableItem_ABC(ADN_Table* parent,void *data, QTableItem::EditType nEditType = WhenCurrent );
    virtual ~ADN_TableItem_ABC();

    void* GetData();
    virtual void DoValueChanged() = 0;

    virtual QString key() const;
    
protected:
    void* pData_;
};


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC::GetData
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
inline
void* ADN_TableItem_ABC::GetData()
{
    return pData_;
}



#endif // __ADN_TableItem_ABC_h_