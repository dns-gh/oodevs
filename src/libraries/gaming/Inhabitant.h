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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
    class DotationType;
    class Displayer_ABC;
    class InhabitantType;
    class UrbanObject_ABC;
}

namespace gui
{
    class GLView_ABC;
}

namespace sword
{
    class PopulationCreation;
    class PopulationUpdate;
}

class UrbanModel;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Inhabitant : public gui::EntityImplementation< kernel::Inhabitant_ABC >
                 , public kernel::Updatable_ABC< sword::PopulationUpdate >
                 , public kernel::Positions
                 , public tools::Observer_ABC
                 , public gui::Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Inhabitant( const sword::PopulationCreation& message,
                         kernel::Controller& controller,
                         const UrbanModel& model,
                         const kernel::InhabitantType& type,
                         const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                         const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;

    virtual geometry::Point2f GetPosition( bool aggregated = false ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Helpers
    //@{
    void DoUpdate( const sword::PopulationUpdate& message );
    void CreateDictionary();
    void UpdateUrbanObjectsDictionnary();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, kernel::UrbanObject_ABC* > T_UrbanObjectVector;
    typedef std::map< std::string, unsigned int >       T_MotivationSatisfactions;
    typedef std::map< const kernel::DotationType*, unsigned int > T_ResourceSatisfactions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    const unsigned int male_;
    const unsigned int female_;
    const unsigned int children_;
    unsigned int healthSatisfaction_;
    unsigned int safetySatisfaction_;
    unsigned int lodgingSatisfaction_;
    T_MotivationSatisfactions motivationSatisfactions_;
    T_ResourceSatisfactions resourceSatisfactions_;
    T_UrbanObjectVector livingUrbanObject_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f position_;
    std::string motivation_;
    unsigned int livingUrbanObjects_;
    unsigned int nominalCapacity_;
    unsigned int infrastructures_;
    QMap< QString, unsigned int > accomodationCapacties_;
    //@}
};

#endif // __Inhabitant_h_
