// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ListView_Missions.h $
// $Author: Nld $
// $Modtime: 29/04/05 16:26 $
// $Revision: 5 $
// $Workfile: ADN_ListView_Missions.h $
//
// *****************************************************************************

#ifndef __ADN_ListView_Missions_h_
#define __ADN_ListView_Missions_h_

#include "ADN_Types.h"
#include "ADN_ListView.h"

class ADN_ListView_Models;
class QPopupMenu;

enum E_SMission;

// =============================================================================
/** @class  ADN_ListView_Missions
    @brief  ADN_ListView_Missions
*/
// Created: AGN 2003-11-27
// =============================================================================
class ADN_ListView_Missions
: public ADN_ListView
{
public:
    explicit ADN_ListView_Missions( bool bForAutomata, ADN_ListView_Models* pList, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    virtual ~ADN_ListView_Missions();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );

    void FillList( QCheckListItem* pParent, E_SMission eStart, E_SMission eEnd );
    ADN_ListViewItem* FindItem( const std::string& strMissionName );
    void ApplyModifications( QCheckListItem* pStart );

private:
    ADN_ListView_Models* pLVModels_;

    bool bForAutomata_;
};

#endif // __ADN_ListView_Missions_h_
