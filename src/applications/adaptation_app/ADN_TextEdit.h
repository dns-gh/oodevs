// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TextEdit_h_
#define __ADN_TextEdit_h_

#include "ADN_Connector_String.h"
#include "ADN_TextEdit_ABC.h"

//*****************************************************************************
// Created: JSR 21-11-11
//*****************************************************************************
template < class Connector >
class ADN_TextEdit : public ADN_TextEdit_ABC
{
public:
    explicit ADN_TextEdit( QWidget* parent, const char * name = 0 );
    virtual ~ADN_TextEdit();

protected:
    virtual void TextChanged();
    virtual void UpdateEnableState();
};

#include "ADN_TextEdit.inl"

typedef ADN_TextEdit< ADN_Connector_String< ADN_TextEdit_ABC > >    ADN_TextEdit_String;

#endif // __ADN_TextEdit_h_