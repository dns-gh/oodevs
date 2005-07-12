//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMaintenanceConsign_ListView.h $
// $Author: Nld $
// $Modtime: 30/12/04 11:31 $
// $Revision: 1 $
// $Workfile: MOS_LogMaintenanceConsign_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_LogMaintenanceConsign_ListView_h_
#define __MOS_LogMaintenanceConsign_ListView_h_

#include <QListView.h>

class MOS_LogMaintenanceConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogMaintenanceConsign_ListView : public QListView
{
public:
    explicit MOS_LogMaintenanceConsign_ListView( QWidget* pParent, bool bHandledConsigns );
    virtual ~MOS_LogMaintenanceConsign_ListView();

    void AddConsign       ( MOS_LogMaintenanceConsign& consign );
    void RemoveConsign    ( MOS_LogMaintenanceConsign& consign );

private:
    bool bHandledConsigns_;
};

#include "MOS_LogMaintenanceConsign_ListView.inl"


#endif // __MOS_LogMaintenanceConsign_ListView_h_