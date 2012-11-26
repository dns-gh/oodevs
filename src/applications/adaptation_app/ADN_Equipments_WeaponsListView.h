// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_WeaponsListView_h_
#define __ADN_Equipments_WeaponsListView_h_

#include "ADN_ListView.h"

#include "ADN_Weapons_Data.h"

// =============================================================================
/** @class  ADN_Equipments_WeaponsListView
    @brief  ADN_Equipments_WeaponsListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_Equipments_WeaponsListView : public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Equipments_WeaponsListView( QWidget* pParent );
    virtual ~ADN_Equipments_WeaponsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Weapons_Data::WeaponInfos* pInfo );
};

#endif // __ADN_Equipments_WeaponsListView_h_