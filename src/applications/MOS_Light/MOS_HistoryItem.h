//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_HistoryItem.h $
//
//*****************************************************************************

#ifndef __MOS_HistoryItem_h_
#define __MOS_HistoryItem_h_

#include "MOS_Types.h"
#include <QTable.h>
#include "MOS_Order_Def.h"

#include "MOS_HistoryMission.h"

class MOS_Agent;
class MOS_HistoryMission;
class MOS_History;



//*****************************************************************************
// Created: FBD 03-08-25
//*****************************************************************************
class MOS_HistoryItem : public QTableItem
{
    MT_COPYNOTALLOWED( MOS_HistoryItem )

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_HistoryMission* >          T_MOS_HistoryMissionVector;
    typedef const T_MOS_HistoryMissionVector            CT_MOS_HistoryMissionVector;
    typedef T_MOS_HistoryMissionVector::iterator        IT_MOS_HistoryMissionVector;
    typedef T_MOS_HistoryMissionVector::const_iterator  CIT_MOS_HistoryMissionVector;
    //@}

public:
    explicit MOS_HistoryItem( MOS_History* pTable, MOS_Agent& agent );
    virtual ~MOS_HistoryItem();

    const MOS_Agent&    GetAgent() const;
    uint                GetWidth() const;
    MOS_HistoryMission& AddMission( uint nTimeStart, uint nTimeEnd, std::string& sName, std::string& sParam, E_MissionID nMission );
    void                DelMission( MOS_HistoryMission& mission );
    void                paint( QPainter * p, const QColorGroup & cg, const QRect & cr, bool selected );
    QRect*              tip( const QPoint& p, std::string& sParam );
    void                SetLastOrderState( E_StateMissionHistory nState, uint nTime );

private:

    MOS_Agent* pAgent_;

    T_MOS_HistoryMissionVector missionVector_;
    int nTotalTime_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_HistoryItem.inl"
#endif


#endif // __MOS_HistoryItem_h_