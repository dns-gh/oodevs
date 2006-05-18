//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "Resolver.h"
#include "Entity_ABC.h"
#include "Extension_ABC.h"
#include "Drawable_ABC.h"

class Controller;
class CoordinateConverter_ABC;
class Team;
class Population;
class TypePopulation;
class Displayer_ABC;
class PopulationPartKnowledge_ABC;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class KnowledgeGroup;

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
             PopulationKnowledge( const KnowledgeGroup& group, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Population >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    std::string GetName() const;

    void Display( Displayer_ABC& displayer ) const;
    void DisplayInList( Displayer_ABC& displayer ) const;
    bool IsInTeam( const Team& team ) const;
    bool KnowledgeIsInTeam( const Team& team ) const;
    const Team* GetKnowledgeTeam() const;

    const KnowledgeGroup& GetKnowledgeGroup() const;
    const Population& GetRealPopulation() const;

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
    const KnowledgeGroup& group_;
    Controller& controller_;
    const CoordinateConverter_ABC& converter_;

    const Population& popu_;
    unsigned long nID_;
    //@}
};

#endif // __PopulationKnowledge_h_