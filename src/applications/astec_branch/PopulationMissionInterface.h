// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationMissionInterface_h_
#define __PopulationMissionInterface_h_

#include "MissionInterface_ABC.h"

class Mission;
class ASN_MsgPionOrder;
class ASN_MsgPopulationOrder;
class Population_ABC;
class ObjectTypes;
class Publisher_ABC;

// =============================================================================
// Created: APE 2004-04-20
// =============================================================================
class PopulationMissionInterface : public MissionInterface_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationMissionInterface( QWidget* parent, Population_ABC& population, unsigned long nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter, const ObjectTypes& objectTypes, Publisher_ABC& publisher );
    virtual ~PopulationMissionInterface();
    //@}
   
private:
    //! @name Helpers
    //@{
    void CreateDefaultParameters();
    void CreateInterface();
    //@}

    //! @name Generated helpers
    //@{
     #include "PopulationMissionInterface_Gen.h" 
    //@}

    //! @name Slots
    //@{
    virtual void OnOk();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationMissionInterface( const PopulationMissionInterface& );
    PopulationMissionInterface& operator=( const PopulationMissionInterface& );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    uint nMissionId_;
    ASN_MsgPopulationOrder* pASNMsgOrder_;
    //@}
};


#endif // __PopulationMissionInterface_h_
