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
class ADN_Composantes_BreakdownsTable
: public ADN_Table2
{
    Q_OBJECT

public:
    explicit ADN_Composantes_BreakdownsTable( const std::string& strName, QWidget* pParent = 0 );
    virtual ~ADN_Composantes_BreakdownsTable();

public slots:
    void OnModified();

private:
    virtual void OnContextMenu( int nRow, int nCol, const QPoint& pt );
};

#endif // __ADN_Composantes_BreakdownsTable_h_
