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
             ADN_Equipments_Dotations_GUI( const QString& objectName, ADN_Connector_ABC*& connector,
                                            bool bIncludeNormalizedConsumption, QWidget* pParent = 0, bool bIncludeThreshold = true );
    virtual ~ADN_Equipments_Dotations_GUI();

private:
    virtual void OnContextMenu( const QPoint& pt );
    void AddNewDotation( ADN_Resources_Data::CategoryInfo& category );
    void RemoveCurrentDotation();
    bool Contains( ADN_Resources_Data::CategoryInfo& category );
    virtual void AddRow( int row, void* data );

public:
    bool bIncludeNormalizedConsumption_;
    bool bIncludeThreshold_;
};

#endif // __ADN_Equipments_Dotations_GUI_h_