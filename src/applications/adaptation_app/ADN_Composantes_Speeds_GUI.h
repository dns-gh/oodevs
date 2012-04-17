// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Composantes_Speeds_GUI_h_
#define __ADN_Composantes_Speeds_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-07-15
//*****************************************************************************
class ADN_Composantes_Speeds_GUI
    : public ADN_Table2
{
    Q_OBJECT

public:
    explicit ADN_Composantes_Speeds_GUI( QLineEdit* maxSpeed, QWidget* parent = 0 );
    virtual ~ADN_Composantes_Speeds_GUI();

public slots:
    void OnItemSelected( void* pData );
    void OnMaxSpeedFinishedEditing();

private:
    bool UpdateSpeedsValidator( double maxSpeed );

private:
    QLineEdit* maxSpeed_;
    bool popupIsDisplayed_;
    double oldMaxSpeed_;
};

#endif // __ADN_Composantes_Speeds_GUI_h_
