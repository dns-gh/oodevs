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

namespace kernel
{
    class Mission;
    class ObjectTypes;
    class Automat_ABC;
}

class ASN_MsgAutomateOrder;
class Publisher_ABC;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class AutomateMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomateMissionInterface( QWidget* parent, kernel::Entity_ABC& agent, uint nMissionId, kernel::ActionController& controller, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const kernel::ObjectTypes& objectTypes, Publisher_ABC& publisher );
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
    kernel::Entity_ABC& agent_;
    uint                  nMissionId_;
    ASN_MsgAutomateOrder* pASNMsgOrder_;
    //@}
};

#endif // __AutomateMissionInterface_h_
