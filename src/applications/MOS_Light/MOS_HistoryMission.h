//*****************************************************************************
//
// $Created: FBD 03-08-25 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_HistoryMission.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_HistoryMission.h $
//
//*****************************************************************************

#ifndef __MOS_HistoryMission_h_
#define __MOS_HistoryMission_h_

#include "MOS_Types.h"
#include "MOS_Order_Def.h"


enum E_StateMissionHistory
{
    eMissionSended = 0,
    eMissionError,
    eMissionStarted,
    eMissionCanceled,
    eMissionStoped
};


//*****************************************************************************
// Created: FBD 03-08-25
//*****************************************************************************
class MOS_HistoryMission
{
    MT_COPYNOTALLOWED( MOS_HistoryMission )

public:
    explicit MOS_HistoryMission( uint nTimeStart, uint nTimeEnd, std::string& sName, std::string& sParam, E_MissionID nMission );
    virtual ~MOS_HistoryMission();

    const std::string&      GetName() const;
    const std::string&      GetParam() const;
    uint                    GetTimeStart() const;
    uint                    GetTimeEnd() const;
    uint                    GetColor() const;
    uint                    GetColorState() const;
    QRect&                  GetRect();
    void                    SetRect( QRect& rect );
    void                    SetTimeEnd( uint nTime );
    void                    SetOrderState( E_StateMissionHistory nState, uint nTime );
    E_StateMissionHistory   GetStateOrder() const;

private:

    std::string             sName_;
    std::string             sParam_;
    uint                    nTimeStart_;
    uint                    nTimeEnd_;
    uint                    nColor_;
    QRect                   rect_;
    E_MissionID             nMission_;
    E_StateMissionHistory   nState_;

};

#ifdef MOS_USE_INLINE
#	include "MOS_HistoryMission.inl"
#endif


#endif // __MOS_HistoryMission_h_