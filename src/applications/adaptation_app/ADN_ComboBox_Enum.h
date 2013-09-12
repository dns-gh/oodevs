// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ComboBox_Enum_h_
#define __ADN_ComboBox_Enum_h_

#include "ADN_ComboBox.h"

//*****************************************************************************
// Created: JDY 03-08-29
//*****************************************************************************
class ADN_ComboBox_Enum : public ADN_ComboBox
{
public:
    explicit ADN_ComboBox_Enum( QWidget * parent = 0 );
    virtual ~ADN_ComboBox_Enum();

protected:
    virtual void SetCurrentData( void* data );
};

#endif // __ADN_ComboBox_Enum_h_