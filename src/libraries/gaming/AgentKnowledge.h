// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_h_
#define __AgentKnowledge_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Types.h"
#include "protocol/Protocol.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Displayer_ABC;
    class Karma;
    class Profile_ABC;
    class Team_ABC;
}

// =============================================================================
/** @class  AgentKnowledge
    @brief  Represents the knowledge a gtia has of an agent.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentKnowledge : public gui::EntityImplementation< kernel::AgentKnowledge_ABC >
                     , public kernel::Extension_ABC
                     , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
                     , public kernel::Displayable_ABC
                     , public gui::Drawable_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             AgentKnowledge( const kernel::KnowledgeGroup_ABC& group,
                             const sword::UnitKnowledgeCreation& message,
                             kernel::Controller& controller,
                             const kernel::CoordinateConverter_ABC& converter,
                             const tools::Resolver_ABC< kernel::Agent_ABC >& resolver,
                             const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver,
                             const kernel::Profile_ABC& profile );
    virtual ~AgentKnowledge();
    //@}

    //! @name operations
    //@{
    virtual void Display         ( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;

    virtual const kernel::Agent_ABC* GetEntity() const;
    virtual unsigned long GetEntityId() const;
    virtual const kernel::Team_ABC* GetTeam() const;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const;
    virtual std::string GetSymbol() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& message );
    void UpdateSymbol();
    const kernel::Karma& TeamKarma( kernel::E_PerceptionResult perception ) const;
    //@}

public:
    const kernel::CoordinateConverter_ABC& converter_;
    const tools::Resolver_ABC< kernel::Agent_ABC >& resolver_;
    const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver_;
    const kernel::KnowledgeGroup_ABC& group_;
    const kernel::Profile_ABC& profile_;

    kernel::Agent_ABC&      realAgent_;
    const kernel::Team_ABC* team_;

    std::string fullSymbol_;
    std::string moveSymbol_;
    std::string staticSymbol_;
    std::string currentSymbol_;
    std::string currentNature_;

    // $$$$ AGE 2006-02-22: Use Attributes ?
    kernel::OptionalValue< std::string >                strPosition_;
    kernel::OptionalValue< uint >                       nDirection_;
    kernel::OptionalValue< uint >                       nSpeed_;
    kernel::OptionalValue< uint >                       nEtatOps_;
    kernel::OptionalValue< kernel::E_PerceptionResult > nCurrentPerceptionLevel_;
    kernel::OptionalValue< kernel::E_PerceptionResult > nMaxPerceptionLevel_;
    kernel::OptionalValue< E_NatureLevel >              nLevel_;
    kernel::OptionalValue< bool >                       bIsPC_;
    kernel::OptionalValue< const kernel::Team_ABC* >    surrenderedTo_;
    kernel::OptionalValue< bool >                       bPrisonner_;
    kernel::OptionalValue< bool >                       bRefugies_;
    kernel::OptionalValue< uint >                       nRelevance_;
    kernel::OptionalValue< std::string >                criticalIntelligence_;
    kernel::OptionalValue< E_UnitPosture >              posture_;
};

#endif // __AgentKnowledge_h_