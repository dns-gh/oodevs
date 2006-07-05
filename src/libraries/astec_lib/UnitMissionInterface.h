// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitMissionInterface_h_
#define __UnitMissionInterface_h_

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgPionOrder;
class Agent;
class ObjectTypes;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class UnitMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitMissionInterface( QWidget* parent, Agent& agent, uint nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes );
    virtual ~UnitMissionInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UnitMissionInterface( const UnitMissionInterface& );
    UnitMissionInterface& operator=( const UnitMissionInterface& );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}

    //! @name Generated helpers
    //@{
#include "UnitMissionInterface_Gen.h" 
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnOk();
    //@}

private:
    //! @name Member data
    //@{
    Agent& agent_;
    uint nMissionId_;
    ASN_MsgPionOrder* pASNMsgOrder_;
    //@}
};

#endif // __UnitMissionInterface_h_
