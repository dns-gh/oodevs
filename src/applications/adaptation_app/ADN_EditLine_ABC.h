// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_EditLine_ABC_h_
#define __ADN_EditLine_ABC_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_BaseEditLine.h"

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
class ADN_EditLine_ABC : public ADN_BaseEditLine
                       , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_EditLine_ABC( QWidget* parent, const char * name = 0 );
    virtual ~ADN_EditLine_ABC();

    void setEnabled( bool b );

protected:
    void focusOutEvent( QFocusEvent* pEvent );

protected slots:
    virtual void TextChanged( const QString& string )=0;
    virtual void UpdateEnableState() = 0;
};

#endif // __ADN_EditLine_ABC_h_