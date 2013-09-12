// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TextEdit_ABC_h_
#define __ADN_TextEdit_ABC_h_

#include "ADN_Gfx_ABC.h"

class ADN_Ref_ABC;

//*****************************************************************************
// Created: JSR 21-11-11
//*****************************************************************************
class ADN_TextEdit_ABC : public QTextEdit
                       , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_TextEdit_ABC( QWidget* parent = 0, const char * name = 0 );
    virtual ~ADN_TextEdit_ABC();

    void setEnabled( bool b );
    void SetToolTip( const QString& toolTip );
    void ConnectWithRefValidity( const ADN_Ref_ABC& ref );

protected slots:
    virtual void TextChanged() = 0;
    virtual void UpdateEnableState() = 0;
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );

private:
    const QPalette originalPalette_;
    QString originalToolTip_;
};

#endif // __ADN_TextEdit_ABC_h_