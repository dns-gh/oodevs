// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_EditLine_h_
#define __ADN_EditLine_h_

#include "ADN_Validator.h"
#include "ADN_EditLine_ABC.h"
#include "ADN_Connector_String.h"
#include "ADN_Connector_Int.h"
#include "ADN_Connector_Double.h"
#include "ADN_Connector_LocalizedString.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_WorkspaceElement.h"

//*****************************************************************************
// Created: JDY 03-07-01
//*****************************************************************************
template < class Connector, class Validator >
class ADN_EditLine : public ADN_EditLine_ABC
{
public:
    explicit ADN_EditLine( QWidget* parent, const char * name = 0 );
    virtual ~ADN_EditLine();

    Validator& GetValidator();

protected:
    virtual void TextChanged( const QString& string );
    virtual void mousePressEvent( QMouseEvent* event );

private:
    std::unique_ptr< Validator > pValidator_;
};

#include "ADN_EditLine.inl"

typedef ADN_EditLine< ADN_Connector_String< ADN_EditLine_ABC >, QRegExpValidator >          ADN_EditLine_String;
typedef ADN_EditLine< ADN_Connector_Int< ADN_EditLine_ABC >,    ADN_IntValidator >          ADN_EditLine_Int;
typedef ADN_EditLine< ADN_Connector_Double< ADN_EditLine_ABC >, ADN_DoubleValidator >       ADN_EditLine_Double;
typedef ADN_EditLine< ADN_Connector_LocalizedString< ADN_EditLine_ABC >, QRegExpValidator > ADN_EditLine_LocalizedString;

// -----------------------------------------------------------------------------
// Name: ADN_PH_EditLine_Double
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
class ADN_PH_EditLine_Double : public ADN_EditLine_Double
{
public:
    explicit ADN_PH_EditLine_Double(QWidget* parent, const char * name = 0)
        : ADN_EditLine_Double( parent, name ) {}

protected:
    virtual void TextChanged(const QString& string)
    {
        ADN_EditLine_Double::TextChanged( string );
        ADN_Workspace::GetWorkspace().GetWeapons().GetGui().UpdateModifiers();
    }
};

#endif // __ADN_EditLine_h_