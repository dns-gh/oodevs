// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_AttritionTable_h_
#define __ADN_Resources_AttritionTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Resources_AttritionTable
    @brief  ADN_Resources_AttritionTable
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Resources_AttritionTable : public ADN_Table
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Resources_AttritionTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Resources_AttritionTable();
    //@}

public:
    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    //@}
};

#endif // __ADN_Resources_AttritionTable_h_
