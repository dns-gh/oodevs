// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Agent_ABC_h_
#define __MOS_Agent_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include "MOS_FireResult.h"

class MOS_Report_ABC;
class MOS_AgentKnowledge;
class MOS_Team;
class MOS_Report_ABC;
class MOS_TacticalLine_ABC;

// =============================================================================
// Created: HME 2005-10-05
// =============================================================================
class MOS_Agent_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< MOS_Report_ABC* >     T_ReportVector;
    typedef T_ReportVector::iterator           IT_ReportVector;
    typedef T_ReportVector::const_iterator     CIT_ReportVector;

    typedef std::map< uint, ASN1T_MsgPionOrder >      T_MissionOrders;
    typedef T_MissionOrders::iterator                 IT_MissionOrders;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Agent_ABC();
    virtual ~MOS_Agent_ABC();
    //@}

    //! @name Accessors
    //@{
	virtual const MIL_AgentID   GetID           () const = 0;
	virtual const std::string   GetName         () const = 0;
    virtual const MT_Vector2D&  GetPos          () const = 0;
    virtual const MT_Vector2D   GetAggregatedPos() const = 0;
    virtual const MT_Vector2D   GetConflictPos  ( const MT_Vector2D& origin ) const;
    virtual MOS_Team&           GetTeam         () const = 0;
    //@}

    //! @name Reports
    //@{
    T_ReportVector& GetReports                          ();
    void            DeleteAllRCs                        ();
    void            DeleteAllTraces                     ();
    void            DeleteReport                        ( MOS_Report_ABC& );

    void            OnReceiveMsgCR                      ( const ASN1T_MsgCR& msg );
    void            OnReceiveTraceMsg                   ( DIN::DIN_Input&    msg );

    void            OnReceiveMsgWaitForOrderConduite    ( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
	int             GetCurrentMission                   () const;
	void            OnReceiveMission                    ( const ASN1T_MsgPionOrder& asnMsg );
    void            OnReceiveMission                    ( const ASN1T_MsgPionOrderAck& asnMsg );
    void            OnSendMissionOrder                  ( ASN1T_MsgPionOrder order );
    
    MOS_TacticalLine_ABC* GetRightLimit           () const;
    MOS_TacticalLine_ABC* GetLeftLimit            () const;
    //@}

    //! @name Debug draw points
    //@{
    const std::vector< MT_Vector2D >& GetDebugDrawPoints         () const;
    void                              OnReceiveDebugDrawPointsMsg( DIN::DIN_Input& msg );
    //@}

    //! @name Fire results
    //@{
    const T_FireResults& GetFireResult       () const;
    void                 OnReceiveMsgStopFire( const ASN1T_FireDamagesPion&       asnMsg );
    void                 OnReceiveMsgStopFire( const ASN1T_FireDamagesPopulation& asnMsg );
    void                 DeleteAllFireResults();
    //@}

    //! @name Knowledge
    //@{
    virtual MOS_AgentKnowledge*  FindAgentKnowledge( uint nId ) = 0;
    //@}
public:
    bool    bListened_;
protected:
    T_ReportVector				reportVector_;

private:
    std::vector< MT_Vector2D >	reportPoints_;
    T_FireResults				fireResults_;

    int nCurrentMission_;
    T_MissionOrders sentOrders_;
    int nCurrentRightLimit_;
    int nCurrentLeftLimit_;
    
};


#include "MOS_Agent_ABC.inl"

#endif // __MOS_Agent_ABC_h_
