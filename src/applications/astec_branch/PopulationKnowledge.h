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

class Controller;
class Team;
class Population;
class TypePopulation;
class Displayer_ABC;
class PopulationPartKnowledge_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public Resolver< PopulationPartKnowledge_ABC >
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( Controller& controller, const Resolver_ABC< Population >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    void Display( Displayer_ABC& displayer ) const;
    void DisplayInList( Displayer_ABC& displayer ) const;
    unsigned long GetId() const;
    bool IsInTeam( const Team& team ) const;
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
    Controller& controller_;
    const Population& popu_;
    unsigned long nID_;
};

#endif // __PopulationKnowledge_h_