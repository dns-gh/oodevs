// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TableItem_ABC.h"

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_TableItem_ABC::ADN_TableItem_ABC( QTable* parent, void* data, QTableItem::EditType nEditType /* = WhenCurrent*/ )
: QTableItem( parent, nEditType, "" )
, QObject( parent )
, pData_( data )
{
    setReplaceable( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_TableItem_ABC::~ADN_TableItem_ABC()
{
    // NOTHING
}
