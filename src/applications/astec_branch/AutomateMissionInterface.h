// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AutomateMissionInterface.h $
// $Author: Ape $
// $Modtime: 25/10/04 10:45 $
// $Revision: 2 $
// $Workfile: AutomateMissionInterface.h $
//
// *****************************************************************************

#ifndef __AutomateMissionInterface_h_
#define __AutomateMissionInterface_h_

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgAutomateOrder;
class Agent;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class AutomateMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateMissionInterface( QWidget* parent, Agent& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter );
    virtual ~AutomateMissionInterface();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}

    //! @name Generated helpers
    //@{
    #include "AutomateMissionInterface_Gen.h" 
    //@}

private:
    //! @name Slots
    //@{
    void OnOk();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomateMissionInterface( const AutomateMissionInterface& );
    AutomateMissionInterface& operator=( const AutomateMissionInterface& );
    //@}

private:
    //! @name Member data
    //@{
    Agent&                agent_;
    uint                  nMissionId_;
    ASN_MsgAutomateOrder* pASNMsgOrder_;
    //@}
};

#endif // __AutomateMissionInterface_h_
