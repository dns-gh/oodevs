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
    virtual void UpdateEnableState();

private:
    Validator* pValidator_;
};

#include "ADN_EditLine.inl"

typedef ADN_EditLine< ADN_Connector_String< ADN_EditLine_ABC >, QRegExpValidator >    ADN_EditLine_String;
typedef ADN_EditLine< ADN_Connector_Int< ADN_EditLine_ABC >,    ADN_IntValidator >    ADN_EditLine_Int;
typedef ADN_EditLine< ADN_Connector_Double< ADN_EditLine_ABC >, ADN_DoubleValidator > ADN_EditLine_Double;

#endif // __ADN_EditLine_h_