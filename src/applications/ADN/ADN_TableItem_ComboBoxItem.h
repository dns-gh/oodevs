//*****************************************************************************
//
// $Created: JDY 03-09-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_ComboBoxItem.h $
// $Author: Ape $
// $Modtime: 28/01/05 11:35 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_ComboBoxItem.h $
//
//*****************************************************************************

#ifndef __ADN_TableItem_ComboBoxItem_h_
#define __ADN_TableItem_ComboBoxItem_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_TableItem_ComboBox.h"

class ADN_TableItem_ComboBox;

//*****************************************************************************
// Created: JDY 03-09-10
//*****************************************************************************
class ADN_TableItem_ComboBoxItem
:   public ADN_Gfx_ABC
{
    MT_COPYNOTALLOWED(ADN_TableItem_ComboBoxItem)
public:
    explicit ADN_TableItem_ComboBoxItem(ADN_TableItem_ComboBox& combo,void *data);
    virtual ~ADN_TableItem_ComboBoxItem();

    void*           GetData();
    void            setText(const QString& txt);
    QString         text() const;
    void            setEnabled(bool);

private:

    ADN_TableItem_ComboBox&   combo_;
    void*                     pData_;
    QString                   szTmpTxt_;
};


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ComboBoxItem::GetData
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
inline
void*  ADN_TableItem_ComboBoxItem::GetData()
{
    return pData_;
}


#endif // __ADN_TableItem_ComboBoxItem_h_