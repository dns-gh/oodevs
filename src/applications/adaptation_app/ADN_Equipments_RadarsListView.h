// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_RadarsListView_h_
#define __ADN_Equipments_RadarsListView_h_

#include "ADN_ListView.h"
#include "ADN_Radars_Data.h"

// =============================================================================
/** @class  ADN_Equipments_RadarsListView
    @brief  ADN_Equipments_RadarsListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Equipments_RadarsListView : public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Equipments_RadarsListView( QWidget* pParent );
    virtual ~ADN_Equipments_RadarsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Radars_Data::RadarInfos* pInfo );
};

#endif // __ADN_Equipments_RadarsListView_h_