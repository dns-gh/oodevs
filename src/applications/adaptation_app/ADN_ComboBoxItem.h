// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ComboBoxItem_h_
#define __ADN_ComboBoxItem_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_ComboBox.h"
#include <boost/noncopyable.hpp>

class ADN_ComboBox;

//*****************************************************************************
// Created: JDY 03-09-08
//*****************************************************************************
class ADN_ComboBoxItem : public ADN_Gfx_ABC
                       , private boost::noncopyable
{

public:

    explicit ADN_ComboBoxItem(ADN_ComboBox& combo,void *data);
    virtual ~ADN_ComboBoxItem();

    void*           GetData();
    void            setText(const QString& txt);
    QString         text() const;
    void            setEnabled(bool);

protected:
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