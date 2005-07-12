//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogSupplyConsign_ListView.h $
// $Author: Nld $
// $Modtime: 28/01/05 10:37 $
// $Revision: 1 $
// $Workfile: MOS_LogSupplyConsign_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_LogSupplyConsign_ListView_h_
#define __MOS_LogSupplyConsign_ListView_h_

#include <QListView.h>

class MOS_LogSupplyConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogSupplyConsign_ListView : public QListView
{
public:
    explicit MOS_LogSupplyConsign_ListView( QWidget* pParent, bool bHandledConsigns );
    virtual ~MOS_LogSupplyConsign_ListView();

    void AddConsign       ( MOS_LogSupplyConsign& consign );
    void RemoveConsign    ( MOS_LogSupplyConsign& consign );

private:
    bool bHandledConsigns_;
};

#include "MOS_LogSupplyConsign_ListView.inl"


#endif // __MOS_LogSupplyConsign_ListView_h_