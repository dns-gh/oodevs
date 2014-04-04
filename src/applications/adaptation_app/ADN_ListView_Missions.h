// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_Missions_h_
#define __ADN_ListView_Missions_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"
#include "ADN_Models_Data.h"
#include "ADN_Missions_Data.h"

enum E_EntityType;

class ADN_ListView_Models;
class ADN_StandardItem;

// =============================================================================
/** @class  ADN_ListView_Missions
    @brief  ADN_ListView_Missions
*/
// Created: AGN 2003-11-27
// =============================================================================
class ADN_ListView_Missions : public ADN_ListView
{

public:
             ADN_ListView_Missions( E_EntityType eEntityType, QWidget* parent );
    virtual ~ADN_ListView_Missions();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    void FillList( QTreeWidgetItem* pParent, ADN_Missions_Data::T_Mission_Vector& missions );
    ADN_StandardItem* FindItem( const std::string& strMissionName );
    void ApplyModifications( QTreeWidgetItem* pStart );

private:
    E_EntityType eEntityType_;
};

#endif // __ADN_ListView_Missions_h_
