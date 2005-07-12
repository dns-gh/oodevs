//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_History.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_History.h $
//
//*****************************************************************************

#ifndef __MOS_History_h_
#define __MOS_History_h_


#include "MOS_Types.h"
#include <QTable.h>
#include <QToolTip.h>
#include "MOS_Order_Def.h"

#include "MOS_HistoryMission.h"

class MOS_HistoryItem;
class MOS_Agent;

class DynamicTip : public QToolTip
{
public:
    DynamicTip( QWidget * parent );

protected:
    void maybeTip( const QPoint & );
};

//*****************************************************************************
// Created: FBD 03-08-25
//*****************************************************************************
class MOS_History : public QTable
{
    MT_COPYNOTALLOWED( MOS_History )

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_HistoryItem* >          T_HistoryItemVector;
    typedef const T_HistoryItemVector            CT_HistoryItemVector;
    typedef T_HistoryItemVector::iterator        IT_HistoryItemVector;
    typedef T_HistoryItemVector::const_iterator  CIT_HistoryItemVector;
    //@}

public:
    explicit MOS_History( QWidget* pParent );
    virtual ~MOS_History();
    void AddAgent( MOS_Agent& agent );
    void DelAgent( MOS_Agent& agent );
    void AddMission( MOS_Agent& agent, std::string& sName, uint nTimeStart, uint nTimeEnd, std::string& sParam, E_MissionID nMission );
    void SetLastOrderState( MOS_Agent& agent, E_StateMissionHistory nState, uint nTime );
    void Update();
    void Reset();
    QRect tip( const QPoint& p, std::string& sParam );
    void StartPopup( std::string& sParam );

private:
    DynamicTip* pTip_;
    T_HistoryItemVector itemPtrVector_;
};


#ifdef MOS_USE_INLINE
#	include "MOS_History.inl"
#endif


#endif // __MOS_History_h_