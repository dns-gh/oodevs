// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Dotations_GUI_h_
#define __ADN_Equipments_Dotations_GUI_h_

#include "ADN_CommonGfx.h"
#include "ADN_Resources_Data.h"

// =============================================================================
/** @class  ADN_Equipments_Dotations_GUI
    @brief  ADN_Equipments_Dotations_GUI
*/
// Created: APE 2005-01-07
// =============================================================================
class ADN_Equipments_Dotations_GUI : public ADN_Table
{
    Q_OBJECT

public:
    enum eColumn
    {
        eColumn_Category      = 0x01,
        eColumn_Quantity      = 0x02,
        eColumn_LowThreshold  = 0x04,
        eColumn_HighThreshold = 0x08,
        eColumn_Consumption   = 0x10,
        eColumn_All           = 0x1F
    };

public:
             ADN_Equipments_Dotations_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0, int visibleColumns = eColumn_All, QHeaderView::ResizeMode resizeMode = QHeaderView::Stretch );
    virtual ~ADN_Equipments_Dotations_GUI();

private:
    virtual void OnContextMenu( const QPoint& pt );
    void AddNewDotation( ADN_Resources_Data::CategoryInfo& category );
    void RemoveCurrentDotation();
    bool Contains( ADN_Resources_Data::CategoryInfo& category );
    virtual void AddRow( int row, void* data );
};

#endif // __ADN_Equipments_Dotations_GUI_h_