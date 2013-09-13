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

class ADN_Ref_ABC;

class ADN_EditLine_ToolTiped : public QLineEdit
{
public:
    explicit ADN_EditLine_ToolTiped( QWidget* parent, const char * name = 0 );
    virtual ~ADN_EditLine_ToolTiped();

protected:
    QString ConvertMinMaxValue( double value, int decimals = -1 );
    virtual bool event( QEvent* event );
};

//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
class ADN_EditLine_ABC : public ADN_EditLine_ToolTiped
                       , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_EditLine_ABC( QWidget* parent, const char * name = 0 );
    virtual ~ADN_EditLine_ABC();

    void SetToolTip( const QString& toolTip );
    void ConnectWithRefValidity( const ADN_Ref_ABC& ref );

protected:
    void focusOutEvent( QFocusEvent* pEvent );
    void focusInEvent( QFocusEvent* pEvent );

signals:
    void StartEditing();
    void TypeChanged( int );
    void OnTypeChanged( int );

protected slots:
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMessage = "" );
    virtual void TextChanged( const QString& string )=0;

private:
    const QPalette originalPalette_;
    QString originalToolTip_;
};

#endif // __ADN_EditLine_ABC_h_
