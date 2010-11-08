// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;    
    class Displayer_ABC;
    class PopulationKnowledge_ABC;
}

class PopulationPartKnowledge_ABC;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public kernel::EntityImplementation< kernel::PopulationKnowledge_ABC >
                          , public tools::Resolver< PopulationFlowKnowledge >
                          , public tools::Resolver< PopulationConcentrationKnowledge >
                          , public kernel::Extension_ABC
                          , public kernel::Drawable_ABC
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdKnowledgeUpdate >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeCreation >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate >
                          , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction >
                          , public kernel::Displayable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( const kernel::KnowledgeGroup_ABC& group, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::Population_ABC >& resolver, const MsgsSimToClient::MsgCrowdKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::Population_ABC* GetEntity() const;
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Network
    //@{
    // $$$$ AGE 2006-03-13: hmmm
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction&          message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledge( const PopulationKnowledge& );
    PopulationKnowledge& operator=( const PopulationKnowledge& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::KnowledgeGroup_ABC& group_;
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;

    const kernel::Population_ABC& popu_;
    const kernel::Team_ABC* pTeam_;
    unsigned int domination_;
    //@}
};

#endif // __PopulationKnowledge_h_