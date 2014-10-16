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

#include "PopulationPartKnowledge_ABC.h"
#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Updatable_ABC.h"
#include <boost/optional.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Population_ABC;
    class Controller;
    class Displayer_ABC;
    class PopulationConcentration_ABC;
}

namespace sword
{
    class CrowdConcentrationKnowledgeCreation;
    class CrowdConcentrationKnowledgeUpdate;
}

class PopulationConcentration;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationConcentrationKnowledge : public PopulationPartKnowledge_ABC
                                       , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeUpdate >
                                       , public gui::Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationConcentrationKnowledge( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Population_ABC& popu,
                 const sword::CrowdConcentrationKnowledgeCreation& message );
    virtual ~PopulationConcentrationKnowledge();
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
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    PopulationConcentrationKnowledge( const PopulationConcentrationKnowledge& );
    PopulationConcentrationKnowledge& operator=( const PopulationConcentrationKnowledge& );
    //@}

private:
    //! @name Helpers
    //@{
    float GetRadius() const;
    const PopulationConcentration* GetConcentration() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Population_ABC& resolver_;
    const uint nID_;

    unsigned int                                   concentrationId_;
    geometry::Point2f                              position_;
    kernel::OptionalValue< uint >                  nNbrAliveHumans_;
    kernel::OptionalValue< uint >                  nNbrDeadHumans_;
    kernel::OptionalValue< E_PopulationAttitude >  eAttitude_;
    kernel::OptionalValue< bool >                  bIsPerceived_;
    float rRelevance_;
    boost::optional< float > radius_;
    float deadRadius_;
    mutable const PopulationConcentration* concentration_;
    //@}
};

#endif // __PopulationConcentrationKnowledge_h_