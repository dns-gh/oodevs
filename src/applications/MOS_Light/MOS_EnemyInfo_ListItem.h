//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_ListItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_EnemyInfo_ListItem.h $
//
//*****************************************************************************

#ifndef __MOS_EnemyInfo_ListItem_h_
#define __MOS_EnemyInfo_ListItem_h_

#include "MOS_Types.h"
#include <QListView.h>

class MOS_EnemyInfo;

//*****************************************************************************
// Created: AGN 03-05-20
//*****************************************************************************
class MOS_EnemyInfo_ListItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_EnemyInfo_ListItem )
public:
    explicit MOS_EnemyInfo_ListItem( QListView* pParent, const std::string& strLabel, MOS_EnemyInfo& info );
    ~MOS_EnemyInfo_ListItem();

    MOS_EnemyInfo& GetInfo();

protected:
    virtual void okRename( int col );
private:
    MOS_EnemyInfo& info_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_EnemyInfo_ListItem.inl"
#endif


#endif // __MOS_EnemyInfo_ListItem_h_