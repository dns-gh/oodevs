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

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Updatable_ABC.h"
#include "PopulationPartKnowledge_ABC.h"

namespace sword
{
    class FlowPart;
    class CrowdFlowKnowledgeCreation;
    class CrowdFlowKnowledgeUpdate;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class Controller;
    class Population_ABC;
    class PopulationFlow_ABC;
}

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationFlowKnowledge : public PopulationPartKnowledge_ABC
                              , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeUpdate >
                              , public gui::Drawable_ABC
{
public:
    //! @name Types
    //@{
    struct FlowPart
    {
        FlowPart( const sword::FlowPart& message, const kernel::CoordinateConverter_ABC& converter, geometry::Rectangle2f& boundingBox );
        T_PointVector flowPart_;
        unsigned short relevance_;
    };

    typedef std::vector< FlowPart >       T_FlowParts;
    typedef T_FlowParts::const_iterator CIT_FlowParts;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             PopulationFlowKnowledge( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Population_ABC& popu, const sword::CrowdFlowKnowledgeCreation& message );
    virtual ~PopulationFlowKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    uint GetNId() const;
    //@}

    //! @name Network
    //@{
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeUpdate& message );
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