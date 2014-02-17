// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Crowds_Speeds_GUI_h_
#define __ADN_Crowds_Speeds_GUI_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_Crowds_Speeds_GUI
    @brief  ADN_Crowds_Speeds_GUI
*/
// Created: JSR 2014-02-13
// =============================================================================
class ADN_Crowds_Speeds_GUI : public ADN_Table
{
    Q_OBJECT
public:
             ADN_Crowds_Speeds_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Crowds_Speeds_GUI();

private:
    virtual void AddRow( int row, void* data );
};

#endif // __ADN_Crowds_Speeds_GUI_h_
