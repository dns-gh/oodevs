//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_List.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_EnemyInfo_List.h $
//
//*****************************************************************************

#ifndef __MOS_EnemyInfo_List_h_
#define __MOS_EnemyInfo_List_h_

#include "MOS_Types.h"

#include "MT/MT_Tools/MT_IdentifierManager.h"
#include <QListView.h>
#include <QObjectDefs.h>

class QWidget;
class MOS_EnemyInfo_Editor;
class MOS_EnemyInfo;
class MOS_EnemyInfo_Sending;
class MOS_Agent;

//*****************************************************************************
// Created: AGN 03-05-20
//*****************************************************************************
class MOS_EnemyInfo_List
: public QListView

{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_EnemyInfo_List )
public:
    explicit MOS_EnemyInfo_List( QWidget* pParent );
    ~MOS_EnemyInfo_List();

    void Draw();

    void NotifyDeleteAgent( MOS_Agent& agent );

protected slots:
    void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );
    void SlotDrawRensEni( bool bNewVal );

private:
    bool bDrawRensEni_;

    MOS_EnemyInfo_Editor* pEditor_;
    MOS_EnemyInfo_Sending* pSender_;
    static MT_IdentifierManager idManager_;

    typedef std::vector< MOS_EnemyInfo* > T_EnemyInfo_Vector;
    typedef T_EnemyInfo_Vector::iterator IT_EnemyInfo_Vector;
    typedef T_EnemyInfo_Vector::const_iterator CIT_EnemyInfo_Vector;

    T_EnemyInfo_Vector enemyInfos_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_EnemyInfo_List.inl"
#endif


#endif // __MOS_EnemyInfo_List_h_