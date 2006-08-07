//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationFlowKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationFlowKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationFlowKnowledge_h_
#define __PopulationFlowKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "OptionalValue.h"
#include "PopulationPartKnowledge_ABC.h"

class CoordinateConverter_ABC;
class PopulationFlow;
class PopulationKnowledge;
class Controller;
class Population;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationFlowKnowledge : public PopulationPartKnowledge_ABC
{
public:
    //! @name Types
    //@{
    struct FlowPart
    {
        FlowPart( ASN1T_PortionFlux& asn, const CoordinateConverter_ABC& converter );
        T_PointVector flowPart_;
        float         rRelevance_;
    };

    typedef std::vector< FlowPart >       T_FlowParts;
    typedef T_FlowParts::const_iterator CIT_FlowParts;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             PopulationFlowKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population& popu, const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg );
    virtual ~PopulationFlowKnowledge();
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
    void Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlowKnowledge( const PopulationFlowKnowledge& );
    PopulationFlowKnowledge& operator=( const PopulationFlowKnowledge& );
    //@}

private:
    Controller& controller_;
    const CoordinateConverter_ABC& converter_;

    const Population& popu_; 
    const uint nID_;
    const PopulationFlow* pFlow_;

    OptionalValue< float >                 rDirection_;
    OptionalValue< float >                 rSpeed_;
    OptionalValue< uint  >                 nNbrAliveHumans_;
    OptionalValue< uint  >                 nNbrDeadHumans_;
    OptionalValue< E_PopulationAttitude >  eAttitude_;
    OptionalValue< bool  >                 bIsPerceived_;
    T_FlowParts                            flowParts_;

};

#endif // __PopulationFlowKnowledge_h_