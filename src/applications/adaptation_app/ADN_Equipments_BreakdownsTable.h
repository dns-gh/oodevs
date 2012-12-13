// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipments_BreakdownsTable_h_
#define __ADN_Equipments_BreakdownsTable_h_

#include "ADN_CommonGfx.h"
#include "ADN_Breakdowns_Data.h"

// =============================================================================
/** @class  ADN_Equipments_BreakdownsTable
    @brief  ADN_Equipments_BreakdownsTable
*/
// Created: APE 2005-04-27
// =============================================================================
class ADN_Equipments_BreakdownsTable : public ADN_Table
{
    Q_OBJECT

public:
             ADN_Equipments_BreakdownsTable( const QString& objectName, const QString& name, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_BreakdownsTable();

private:
    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );
    bool Contains( const ADN_Breakdowns_Data::BreakdownInfo* pInfo );

protected:
    virtual void dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
};

#endif // __ADN_Equipments_BreakdownsTable_h_
