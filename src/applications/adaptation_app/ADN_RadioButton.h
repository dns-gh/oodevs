// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_RadioButton_h_
#define __ADN_RadioButton_h_

#include "ADN_Connector_ABC.h"
#include "ADN_Gfx_ABC.h"
#include <QtGui/QRadioButton>

class ADN_RadioButton : public QRadioButton
                      , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    explicit ADN_RadioButton( QWidget* pParent, const char* szName = 0 );
    virtual ~ADN_RadioButton();

private slots:
    void BoolChanged( bool b );
};

#endif // __ADN_RadioButton_h_
