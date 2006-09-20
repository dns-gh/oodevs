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

#include "ASN_Types.h"
#include "clients_kernel/IDManager.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
    class Team_ABC;
    class Population_ABC;
    class Displayer_ABC;
    class KnowledgeGroup_ABC;
    class InstanciationComplete;
}

class PopulationPartKnowledge_ABC;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public kernel::Entity_ABC
                          , public kernel::Resolver< PopulationConcentrationKnowledge >
                          , public kernel::Resolver< PopulationFlowKnowledge >
                          , public kernel::Updatable_ABC< kernel::InstanciationComplete >
                          , public kernel::Extension_ABC
                          , public kernel::Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( const kernel::KnowledgeGroup_ABC& group, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Resolver_ABC< kernel::Population_ABC >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;

    void Display( kernel::Displayer_ABC& displayer ) const;
    void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    bool IsInTeam( const kernel::Team_ABC& team ) const;
    bool KnowledgeIsInTeam( const kernel::Team_ABC& team ) const;
    const kernel::Team_ABC* GetKnowledgeTeam() const;

    const kernel::KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    const kernel::Population_ABC& GetRealPopulation() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

    //! @name Network
    //@{
    // $$$$ AGE 2006-03-13: hmmm
    void Update( const ASN1T_MsgPopulationKnowledgeUpdate& message );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    message );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      message );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeCreation&             message );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               message );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          message );
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
    unsigned long nID_;
    unsigned int domination_;
    //@}
};

#endif // __PopulationKnowledge_h_