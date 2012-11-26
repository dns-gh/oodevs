// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_ActiveProtectionsListView_h_
#define __ADN_Equipments_ActiveProtectionsListView_h_

#include "ADN_ListView.h"

#include "ADN_ActiveProtections_Data.h"

// =============================================================================
/** @class  ADN_Equipments_ActiveProtectionsListView
    @brief  ADN_Equipments_ActiveProtectionsListView
*/
// Created: FDS 2010-02-24
// =============================================================================
class ADN_Equipments_ActiveProtectionsListView : public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_Equipments_ActiveProtectionsListView( QWidget* pParent );
    virtual ~ADN_Equipments_ActiveProtectionsListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_ActiveProtections_Data::ActiveProtectionsInfos* pInfo );
};

#endif // __ADN_Equipments_ActiveProtectionsListView_h_