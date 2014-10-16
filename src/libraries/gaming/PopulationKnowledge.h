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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Displayer_ABC;
    class PopulationKnowledge_ABC;
}

namespace sword
{
    class CrowdKnowledgeCreation;
    class CrowdKnowledgeUpdate;
    class CrowdConcentrationKnowledgeCreation;
    class CrowdConcentrationKnowledgeUpdate;
    class CrowdConcentrationKnowledgeDestruction;
    class CrowdFlowKnowledgeCreation;
    class CrowdFlowKnowledgeUpdate;
    class CrowdFlowKnowledgeDestruction;
}

class PopulationPartKnowledge_ABC;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public gui::EntityImplementation< kernel::PopulationKnowledge_ABC >
                          , public tools::Resolver< PopulationFlowKnowledge >
                          , public tools::Resolver< PopulationConcentrationKnowledge >
                          , public kernel::Extension_ABC
                          , public kernel::Updatable_ABC< sword::CrowdKnowledgeUpdate >
                          , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeCreation >
                          , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeUpdate >
                          , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeDestruction >
                          , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeCreation >
                          , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeUpdate >
                          , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeDestruction >
                          , public kernel::Displayable_ABC
                          , public gui::Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( const kernel::KnowledgeGroup_ABC& group, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::Population_ABC >& resolver, const sword::CrowdKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual const kernel::Population_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;

    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;

    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

    //! @name Network
    //@{
    // $$$$ AGE 2006-03-13: hmmm
    virtual void DoUpdate( const sword::CrowdKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeDestruction&          message );
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
    std::string criticalIntelligence_;
    //@}
};

#endif // __PopulationKnowledge_h_