// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_Postures_GUI_h_
#define __ADN_Resources_Postures_GUI_h_

#include "ADN_CommonGfx.h"

//*****************************************************************************
// Created: JDY 03-09-29
//*****************************************************************************
class ADN_Resources_Postures_GUI : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Resources_Postures_GUI( const QString& objectName, const QString& strColCaption, ADN_Connector_ABC*& connector,  QWidget* pParent = 0 );
    virtual ~ADN_Resources_Postures_GUI();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_Resources_Postures_GUI_h_