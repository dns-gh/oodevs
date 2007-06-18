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

#include "game_asn/Asn.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "PopulationPartKnowledge_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
    class Population_ABC;
    class PopulationFlow_ABC;
}

class PopulationKnowledge_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationFlowKnowledge : public PopulationPartKnowledge_ABC
                              , public kernel::Drawable_ABC
                              , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowKnowledgeUpdate >
{
public:
    //! @name Types
    //@{
    struct FlowPart
    {
        FlowPart( ASN1T_FlowPart& asn, const kernel::CoordinateConverter_ABC& converter, geometry::Rectangle2f& boundingBox );
        T_PointVector flowPart_;
        float         rRelevance_;
    };

    typedef std::vector< FlowPart >       T_FlowParts;
    typedef T_FlowParts::const_iterator CIT_FlowParts;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             PopulationFlowKnowledge( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Population_ABC& popu, const ASN1T_MsgPopulationFlowKnowledgeCreation& asnMsg );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Network
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeUpdate& asnMsg );
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
    const kernel::PopulationFlow_ABC* pFlow_;

    kernel::OptionalValue< float >                 rDirection_;
    kernel::OptionalValue< float >                 rSpeed_;
    kernel::OptionalValue< uint  >                 nNbrAliveHumans_;
    kernel::OptionalValue< uint  >                 nNbrDeadHumans_;
    kernel::OptionalValue< E_PopulationAttitude >  eAttitude_;
    kernel::OptionalValue< bool  >                 bIsPerceived_;
    T_FlowParts                                    flowParts_;
    geometry::Rectangle2f                          boundingBox_;

};

#endif // __PopulationFlowKnowledge_h_