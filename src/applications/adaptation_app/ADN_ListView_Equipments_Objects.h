// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_Equipments_Objects_h_
#define __ADN_ListView_Equipments_Objects_h_

#include "ADN_ListView.h"
#include "ADN_Objects_Data.h"

// =============================================================================
/** @class  ADN_ListView_Equipments_Objects
    @brief  ADN_ListView_Equipments_Objects
*/
// Created: APE 2005-01-18
// =============================================================================
class ADN_ListView_Equipments_Objects : public ADN_ListView
{
    Q_OBJECT

public:
    explicit ADN_ListView_Equipments_Objects( QWidget* parent );
    virtual ~ADN_ListView_Equipments_Objects();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    bool Contains( const ADN_Objects_Data_ObjectInfos* pInfo );

};

#endif // __ADN_ListView_Equipments_Objects_h_
