// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Displayable_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Controller;
    class PopulationType;
    class CoordinateConverter_ABC;
    class Displayer_ABC;
}

class PopulationPart_ABC;
class PopulationConcentration;
class PopulationFlow;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public kernel::EntityImplementation< kernel::Population_ABC >
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowCreation > // $$$$ AGE 2006-03-13: dégager dans des extensions ?
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowUpdate >
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowDestruction >
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationUpdate >       
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationCreation >
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationUpdate > 
                 , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationDestruction >
                 , public kernel::Drawable_ABC
                 , public kernel::Displayable_ABC
                 , public kernel::Positions
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             Population( const ASN1T_MsgPopulationCreation& asnMsg, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter,
                         const tools::Resolver_ABC< kernel::PopulationType >& typeResolver );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    unsigned int GetLivingHumans() const;
    unsigned int GetDeadHumans() const;

    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f, float adaptiveFactor = 1.f  ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    //@}

    //! @name Accessors
    //@{
    const kernel::PopulationType& GetType() const;
    //@}
    
private:
    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

    //! @name Helpers
    //@{
    unsigned int ComputeLivingHumans() const;
    unsigned int ComputeDeadHumans() const;

    void DoUpdate( const ASN1T_MsgPopulationFlowCreation&             message );
    void DoUpdate( const ASN1T_MsgPopulationFlowUpdate&               message );
    void DoUpdate( const ASN1T_MsgPopulationFlowDestruction&          message );
    void DoUpdate( const ASN1T_MsgPopulationUpdate&                   message );
    void DoUpdate( const ASN1T_MsgPopulationConcentrationCreation&    message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate&      message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& message );

    void ComputeCenter();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&                    controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::PopulationType&          type_;

    geometry::Rectangle2f                  boundingBox_;
    geometry::Point2f                      center_;
    kernel::OptionalValue< int >           nDomination_;
    //@}

private:
    static unsigned long nMaxId_;
};

#endif // __Population_h_
