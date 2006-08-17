//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBoxItem.h $
// $Author: Ape $
// $Modtime: 28/01/05 11:50 $
// $Revision: 2 $
// $Workfile: ADN_ComboBoxItem.h $
//
//*****************************************************************************

#ifndef __ADN_ComboBoxItem_h_
#define __ADN_ComboBoxItem_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_ComboBox.h"

class ADN_ComboBox;

//*****************************************************************************
// Created: JDY 03-09-08
//*****************************************************************************
class ADN_ComboBoxItem
:   public ADN_Gfx_ABC
{
    MT_COPYNOTALLOWED(ADN_ComboBoxItem)

public:

    explicit ADN_ComboBoxItem(ADN_ComboBox& combo,void *data);
    virtual ~ADN_ComboBoxItem();

    void*           GetData();
    void            setText(const QString& txt);
    QString         text() const;
    void            setEnabled(bool);
private:

    ADN_ComboBox&   combo_;
    void*           pData_;
    QString         szTmpTxt_;

};


//-----------------------------------------------------------------------------
// Name: ADN_ComboBoxItem::GetData
// Created: JDY 03-09-10
//-----------------------------------------------------------------------------
inline
void*  ADN_ComboBoxItem::GetData()
{
    return pData_;
}


#endif // __ADN_ComboBoxItem_h_