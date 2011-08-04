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

class ADN_ListView_Models;

// =============================================================================
/** @class  ADN_ListView_Missions
    @brief  ADN_ListView_Missions
*/
// Created: AGN 2003-11-27
// =============================================================================
class ADN_ListView_Missions : public ADN_ListView
{
public:
    explicit ADN_ListView_Missions( ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType, ADN_ListView_Models* pList, QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ListView_Missions();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    void FillList( Q3CheckListItem* pParent, ADN_Missions_Data::T_Mission_Vector& missions );
    ADN_ListViewItem* FindItem( const std::string& strMissionName );
    void ApplyModifications( Q3CheckListItem* pStart );

private:
    ADN_ListView_Models* pLVModels_;
    ADN_Models_Data::ModelInfos::E_ModelEntityType eEntityType_;
    ADN_Missions_Data::T_Mission_Vector* currentMissions_;
};

#endif // __ADN_ListView_Missions_h_
