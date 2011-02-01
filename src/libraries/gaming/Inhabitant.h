// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitant_h_
#define __Inhabitant_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "Simulation.h"
#include "tools/Resolver.h"

namespace gui
{
    class TerrainObjectProxy;
}

namespace kernel
{
    class Controller;
    class InhabitantType;
    class Displayer_ABC;
    class GlTools_ABC;
}

class UrbanModel;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Inhabitant : public kernel::EntityImplementation< kernel::Inhabitant_ABC >
                 , public kernel::Updatable_ABC< sword::PopulationUpdate >
                 , public kernel::Drawable_ABC
                 , public kernel::Positions
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
public:
    //! @name Constructor/Destructor
    //@{
             Inhabitant( const sword::PopulationCreation& message, kernel::Controllers& controllers,
                         const tools::Resolver_ABC< kernel::InhabitantType >& typeResolver, const UrbanModel& model );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual geometry::Point2f GetPosition( bool aggregated = false ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    const kernel::InhabitantType& GetType() const { return type_; }
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Inhabitant( const Inhabitant& );
    Inhabitant& operator=( const Inhabitant& );
    //@}

    //! @name Helpers
    //@{
    void DoUpdate( const sword::PopulationUpdate& message );
    void CreateDictionary( kernel::Controller& controller );
    virtual void NotifyUpdated( const Simulation::sEndTick& tick );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, gui::TerrainObjectProxy* > T_UrbanObjectVector;
    typedef T_UrbanObjectVector::const_iterator              CIT_UrbanObjectVector;
    typedef std::map< std::string, std::string > T_Extensions;
    typedef std::map< std::string, unsigned int > T_MotivationSatisfactions;
    typedef std::map< unsigned int, unsigned int > T_Humans;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::InhabitantType& type_;
    std::set< kernel::Displayer_ABC* > displayers_;
    unsigned int healthSatisfaction_;
    unsigned int safetySatisfaction_;
    unsigned int lodgingSatisfaction_;
    T_MotivationSatisfactions motivationSatisfactions_;
    T_Humans humans_;
    T_UrbanObjectVector livingUrbanObject_;
    T_Extensions extensions_;
    //@}
};

#endif // __Inhabitant_h_
