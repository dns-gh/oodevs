// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_ABC_h_
#define __Agent_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "ASN_Types.h"
#include "FireResult.h"
#include "DinMessageWrapper.h"

class Report_ABC;
class AgentKnowledge;
class Team;
class Report_ABC;
class TacticalLine_ABC;

// =============================================================================
// Created: HME 2005-10-05
// =============================================================================
class Agent_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< Report_ABC* >     T_ReportVector;
    typedef T_ReportVector::iterator           IT_ReportVector;
    typedef T_ReportVector::const_iterator     CIT_ReportVector;

    typedef std::map< uint, ASN1T_MsgPionOrder >      T_MissionOrders;
    typedef T_MissionOrders::iterator                 IT_MissionOrders;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC();
    virtual ~Agent_ABC();
    //@}

    void Update( const ASN1T_MsgCR& msg );
    void Update( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    void Update( const ASN1T_MsgPionOrder& asnMsg );
    void Update( const ASN1T_MsgPionOrderAck& asnMsg );
    void Update( const TraceMessage& msg );
    void Update( const DebugPointsMessage& msg );

    //! @name Accessors
    //@{

    virtual const MIL_AgentID   GetID           () const = 0;
    virtual const std::string   GetName         () const = 0;
    virtual const MT_Vector2D&  GetPos          () const = 0;
    virtual const MT_Vector2D   GetAggregatedPos() const = 0;
    virtual const MT_Vector2D   GetConflictPos  ( const MT_Vector2D& origin ) const;
    virtual Team&           GetTeam         () const = 0;

    //@}

    //! @name Reports
    //@{
    T_ReportVector& GetReports                          ();
    void            DeleteAllRCs                        ();
    void            DeleteAllTraces                     ();
    void            DeleteReport                        ( Report_ABC& );
    
	int             GetCurrentMission                   () const;
	
    void            OnSendMissionOrder                  ( ASN1T_MsgPionOrder order );
    
    TacticalLine_ABC* GetRightLimit           () const;
    TacticalLine_ABC* GetLeftLimit            () const;
    //@}

    //! @name Debug draw points
    //@{
    const std::vector< MT_Vector2D >& GetDebugDrawPoints         () const;
    
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
    virtual AgentKnowledge*  FindAgentKnowledge( uint nId ) = 0;
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


#include "Agent_ABC.inl"

#endif // __Agent_ABC_h_
