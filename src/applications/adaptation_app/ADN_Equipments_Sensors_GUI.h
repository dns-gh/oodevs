// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Sensors_GUI_h_
#define __ADN_Equipments_Sensors_GUI_h_

#include "ADN_CommonGfx.h"
#include "ADN_Sensors_Data.h"

//*****************************************************************************
// Created: JDY 03-07-22
//*****************************************************************************
class ADN_Equipments_Sensors_GUI : public ADN_Table
{
public:
    explicit ADN_Equipments_Sensors_GUI( const QString& objectName, ADN_Connector_ABC*& connector, QWidget* pParent = 0 );
    virtual ~ADN_Equipments_Sensors_GUI();

private:
    virtual void OnContextMenu( const QPoint& pt );
    virtual void AddRow( int row, void* data );

    bool Contains( const ADN_Sensors_Data::SensorInfos* pInfo );
    void CreateNewSensor( int nSensor );
    void RemoveCurrentSensor();

};

#endif // __ADN_Equipments_Sensors_GUI_h_