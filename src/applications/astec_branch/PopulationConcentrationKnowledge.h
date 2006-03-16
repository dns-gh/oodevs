//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationConcentrationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationConcentrationKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationConcentrationKnowledge_h_
#define __PopulationConcentrationKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "OptionalValue.h"
#include "PopulationPartKnowledge_ABC.h"

class CoordinateConverter;
class Population;
class PopulationConcentration;
class PopulationKnowledge;
class Controller;
class Displayer_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationConcentrationKnowledge : public PopulationPartKnowledge_ABC
{
    
public:
    //! @name Constructor/Destructor
    //@{
             PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter& converter, const Population& popu,
                 const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg );
    virtual ~PopulationConcentrationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const;
    virtual void DisplayInList( Displayer_ABC& displayer ) const;
    //@}

    //! @name Accessors
    //@{

    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    PopulationConcentrationKnowledge( const PopulationConcentrationKnowledge& );
    PopulationConcentrationKnowledge& operator=( const PopulationConcentrationKnowledge& );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Population& resolver_;
    const uint nID_;
    
    const PopulationConcentration*         pConcentration_;
    geometry::Point2f                      position_;
    OptionalValue< uint >                  nNbrAliveHumans_;
    OptionalValue< uint >                  nNbrDeadHumans_;
    OptionalValue< E_PopulationAttitude >  eAttitude_;
    OptionalValue< bool >                  bIsPerceived_;
    float                                  rRelevance_;
    //@}
};

#endif // __PopulationConcentrationKnowledge_h_