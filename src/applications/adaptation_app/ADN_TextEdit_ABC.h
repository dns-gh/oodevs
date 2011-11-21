// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TextEdit_ABC_h_
#define __ADN_TextEdit_ABC_h_

#include "ADN_Gfx_ABC.h"

//*****************************************************************************
// Created: JSR 21-11-11
//*****************************************************************************
class ADN_TextEdit_ABC : public QTextEdit
                       , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_TextEdit_ABC( QWidget* parent, const char * name = 0 );
    virtual ~ADN_TextEdit_ABC();

    void setEnabled( bool b );

protected slots:
    virtual void TextChanged() = 0;
    virtual void UpdateEnableState() = 0;
};

#endif // __ADN_TextEdit_ABC_h_