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
#include "astec_kernel/IDManager.h"
#include "astec_kernel/Resolver.h"
#include "astec_kernel/Entity_ABC.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Drawable_ABC.h"

class Controller;
class CoordinateConverter_ABC;
class Team_ABC;
class Population_ABC;
class TypePopulation;
class Displayer_ABC;
class PopulationPartKnowledge_ABC;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class KnowledgeGroup_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public Entity_ABC
                          , public Resolver< PopulationConcentrationKnowledge >
                          , public Resolver< PopulationFlowKnowledge >
                          , public Extension_ABC
                          , public Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( const KnowledgeGroup_ABC& group, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Population_ABC >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual std::string GetName() const;

    void Display( Displayer_ABC& displayer ) const;
    void DisplayInList( Displayer_ABC& displayer ) const;
    bool IsInTeam( const Team_ABC& team ) const;
    bool KnowledgeIsInTeam( const Team_ABC& team ) const;
    const Team_ABC* GetKnowledgeTeam() const;

    const KnowledgeGroup_ABC& GetKnowledgeGroup() const;
    const Population_ABC& GetRealPopulation() const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
    const KnowledgeGroup_ABC& group_;
    Controller& controller_;
    const CoordinateConverter_ABC& converter_;

    const Population_ABC& popu_;
    unsigned long nID_;
    unsigned int domination_;
    //@}
};

#endif // __PopulationKnowledge_h_