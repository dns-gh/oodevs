// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MissionTypes_Table_h_
#define __ADN_MissionTypes_Table_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_MissionTypes_Table
    @brief  ADN_MissionTypes_Table
*/
// Created: LDC 2010-08-19
// =============================================================================
class ADN_MissionTypes_Table : public ADN_Table3
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionTypes_Table( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_MissionTypes_Table();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    //@}
};

#endif // __ADN_MissionTypes_Table_h_
