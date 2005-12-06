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

class MOS_Report_ABC;
class MOS_AgentKnowledge;
class MOS_Team;
class MOS_Report_ABC;

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
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Agent_ABC();
    virtual ~MOS_Agent_ABC();
    //@}

    //! @name Accessors
    //@{
	virtual const MIL_AgentID   GetID  () const = 0;
	virtual const std::string   GetName() const = 0;
    virtual const MT_Vector2D&  GetPos () const = 0;
    virtual MOS_Team&           GetTeam() const = 0;
    //@}

    //! @name Reports
    //@{
    virtual T_ReportVector& GetReports                 ();
    virtual void            DeleteAllRCs               ();
    virtual void            DeleteAllTraces            ();
    virtual void            DeleteReport               ( MOS_Report_ABC& );

    virtual void            OnReceiveMsgCR             ( const ASN1T_MsgCR& msg );
    virtual void            OnReceiveTraceMsg          ( DIN::DIN_Input&    msg );
    virtual void            OnReceiveDebugDrawPointsMsg( DIN::DIN_Input&    msg );
    //@}

    //! @name Knowledge
    //@{
    virtual MOS_AgentKnowledge*  FindAgentKnowledge( uint nId ) = 0;
    //@}

private:
    T_ReportVector				reportVector_;
    std::vector< MT_Vector2D >	reportPoints_;
    
};


#include "MOS_Agent_ABC.inl"

#endif // __MOS_Agent_ABC_h_
