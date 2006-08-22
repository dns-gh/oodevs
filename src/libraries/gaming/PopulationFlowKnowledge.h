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
#include "clients_kernel/IDManager.h"
#include "clients_kernel/OptionalValue.h"
#include "PopulationPartKnowledge_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
    class Population_ABC;
}

class PopulationFlow;
class PopulationKnowledge;

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
        FlowPart( ASN1T_PortionFlux& asn, const kernel::CoordinateConverter_ABC& converter );
        T_PointVector flowPart_;
        float         rRelevance_;
    };

    typedef std::vector< FlowPart >       T_FlowParts;
    typedef T_FlowParts::const_iterator CIT_FlowParts;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             PopulationFlowKnowledge( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Population_ABC& popu, const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
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
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;

    const kernel::Population_ABC& popu_; 
    const uint nID_;
    const PopulationFlow* pFlow_;

    kernel::OptionalValue< float >                 rDirection_;
    kernel::OptionalValue< float >                 rSpeed_;
    kernel::OptionalValue< uint  >                 nNbrAliveHumans_;
    kernel::OptionalValue< uint  >                 nNbrDeadHumans_;
    kernel::OptionalValue< E_PopulationAttitude >  eAttitude_;
    kernel::OptionalValue< bool  >                 bIsPerceived_;
    T_FlowParts                            flowParts_;

};

#endif // __PopulationFlowKnowledge_h_