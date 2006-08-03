// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomateMissionInterface_h_
#define __AutomateMissionInterface_h_

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgAutomateOrder;
class Agent_ABC;
class ObjectTypes;
class Publisher_ABC;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class AutomateMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateMissionInterface( QWidget* parent, Agent_ABC& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher );
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
    Publisher_ABC& publisher_;
    Agent_ABC&                agent_;
    uint                  nMissionId_;
    ASN_MsgAutomateOrder* pASNMsgOrder_;
    //@}
};

#endif // __AutomateMissionInterface_h_
