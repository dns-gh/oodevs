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

#include "game_asn/Asn.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "PopulationPartKnowledge_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Population_ABC;
    class Controller;
    class Displayer_ABC;
    class PopulationConcentration_ABC;
}

class PopulationKnowledge_ABC;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationConcentrationKnowledge : public PopulationPartKnowledge_ABC
                                       , public kernel::Drawable_ABC
                                       , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeUpdate >
{
    
public:
    //! @name Constructor/Destructor
    //@{
             PopulationConcentrationKnowledge( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Population_ABC& popu,
                 const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg );
    virtual ~PopulationConcentrationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

    //! @name Network
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg );
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
    kernel::Controller& controller_;
    const kernel::Population_ABC& resolver_;
    const uint nID_;
    
    const kernel::PopulationConcentration_ABC*         pConcentration_;
    geometry::Point2f                      position_;
    kernel::OptionalValue< uint >                  nNbrAliveHumans_;
    kernel::OptionalValue< uint >                  nNbrDeadHumans_;
    kernel::OptionalValue< E_PopulationAttitude >  eAttitude_;
    kernel::OptionalValue< bool >                  bIsPerceived_;
    float rRelevance_;
    float radius_;
    float deadRadius_;
    //@}
};

#endif // __PopulationConcentrationKnowledge_h_