// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Composantes_BreakdownsTable.h $
// $Author: Nld $
// $Modtime: 11/05/05 10:36 $
// $Revision: 2 $
// $Workfile: ADN_Composantes_BreakdownsTable.h $
//
// *****************************************************************************

#ifndef __ADN_Composantes_BreakdownsTable_h_
#define __ADN_Composantes_BreakdownsTable_h_

#include "ADN_CommonGfx.h"

// =============================================================================
/** @class  ADN_Composantes_BreakdownsTable
    @brief  ADN_Composantes_BreakdownsTable
*/
// Created: APE 2005-04-27
// =============================================================================
class ADN_Composantes_BreakdownsTable : public ADN_Table3
{
    Q_OBJECT

public:
             ADN_Composantes_BreakdownsTable( const QString& objectName, const QString& name, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Composantes_BreakdownsTable();

private:
    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );

protected:
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
};

#endif // __ADN_Composantes_BreakdownsTable_h_
