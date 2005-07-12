//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogMedicalConsign_ListView.h $
// $Author: Nld $
// $Modtime: 11/01/05 19:44 $
// $Revision: 1 $
// $Workfile: MOS_LogMedicalConsign_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_LogMedicalConsign_ListView_h_
#define __MOS_LogMedicalConsign_ListView_h_

#include <QListView.h>

class MOS_LogMedicalConsign;

//*****************************************************************************
// Created: AGN 03-07-21
//*****************************************************************************
class MOS_LogMedicalConsign_ListView : public QListView
{
public:
    explicit MOS_LogMedicalConsign_ListView( QWidget* pParent, bool bHandledConsigns );
    virtual ~MOS_LogMedicalConsign_ListView();

    void AddConsign       ( MOS_LogMedicalConsign& consign );
    void RemoveConsign    ( MOS_LogMedicalConsign& consign );

private:
    bool bHandledConsigns_;
};

#include "MOS_LogMedicalConsign_ListView.inl"


#endif // __MOS_LogMedicalConsign_ListView_h_