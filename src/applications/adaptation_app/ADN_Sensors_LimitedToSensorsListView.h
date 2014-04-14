// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
// LTO
// *****************************************************************************

#ifndef __ADN_Sensors_LimitedToSensorsListView_h_
#define __ADN_Sensors_LimitedToSensorsListView_h_

#include "ADN_ListView.h"
#include "ADN_Radars_Data.h"
#include "ADN_Sensors_Data.h"

// =============================================================================
/** @class  ADN_Sensors_LimitedToSensorsListView
    @brief  ADN_Sensors_LimitedToSensorsListView
*/
// Created: JSR 2010-03-17
// =============================================================================
class ADN_Sensors_LimitedToSensorsListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Sensors_LimitedToSensorsListView( QWidget* pParent );
    virtual ~ADN_Sensors_LimitedToSensorsListView();
    //@}

private:
    //! @name Operations
    //@{
    void OnContextMenu( const QPoint& pt);
    bool Contains( ADN_Sensors_Data::SensorInfos* pInfo );
    bool Contains( ADN_Radars_Data::RadarInfos* pInfo );
   //@}
};

#endif // __ADN_Sensors_LimitedToSensorsListView_h_
