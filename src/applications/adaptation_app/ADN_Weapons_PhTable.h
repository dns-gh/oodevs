// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_PhTable_h_
#define __ADN_Weapons_PhTable_h_

#include "ADN_Table.h"

// =============================================================================
/** @class  ADN_Weapons_PhTable
    @brief  ADN_Weapons_PhTable
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Weapons_PhTable : public ADN_Table
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Weapons_PhTable( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Weapons_PhTable();
    //@}

    //! @name Operations
    //@{
    virtual void AddRow( int row, void* data );
    virtual void OnContextMenu( const QPoint& pt );
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
    //@}
};

#endif // __ADN_Weapons_PhTable_h_
