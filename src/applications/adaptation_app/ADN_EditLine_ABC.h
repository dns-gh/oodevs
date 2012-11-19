// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_EditLine_ABC_h_
#define __ADN_EditLine_ABC_h_

#include "ADN_Gfx_ABC.h"

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

    void setEnabled( bool b );

protected:
    void focusOutEvent( QFocusEvent* pEvent );
    void focusInEvent( QFocusEvent* pEvent );

signals:
    void StartEditing();

protected slots:
    virtual void TextChanged( const QString& string )=0;
    virtual void UpdateEnableState() = 0;
};

#endif // __ADN_EditLine_ABC_h_