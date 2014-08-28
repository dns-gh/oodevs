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

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/OptionalValue.h"

namespace kernel
{
    class Controller;
    class PopulationType;
    class CoordinateConverter_ABC;
    class Displayer_ABC;
}

namespace sword
{
    class CrowdCreation;
    class CrowdUpdate;
    class CrowdFlowCreation;
    class CrowdFlowUpdate;
    class CrowdFlowDestruction;
    class CrowdConcentrationCreation;
    class CrowdConcentrationUpdate;
    class CrowdConcentrationDestruction;
}

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public gui::EntityImplementation< kernel::Population_ABC >
                 , public kernel::Updatable_ABC< sword::CrowdFlowCreation > // $$$$ AGE 2006-03-13: dégager dans des extensions ?
                 , public kernel::Updatable_ABC< sword::CrowdFlowUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdFlowDestruction >
                 , public kernel::Updatable_ABC< sword::CrowdUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationCreation >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdConcentrationDestruction >
                 , public kernel::Positions
                 , public tools::Observer_ABC
                 , public gui::Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const sword::CrowdCreation& message,
                         kernel::Controllers& controllers,
                         const kernel::CoordinateConverter_ABC& converter,
                         const kernel::PopulationType& type,
                         const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual unsigned int GetHealthyHumans() const;
    virtual unsigned int GetWoundedHumans() const;
    virtual unsigned int GetContaminatedHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

    //! @name Helpers
    //@{
    void DoUpdate( const sword::CrowdFlowCreation& message );
    void DoUpdate( const sword::CrowdFlowUpdate& message );
    void DoUpdate( const sword::CrowdFlowDestruction& message );
    void DoUpdate( const sword::CrowdUpdate& message );
    void DoUpdate( const sword::CrowdConcentrationCreation& message );
    void DoUpdate( const sword::CrowdConcentrationUpdate& message );
    void DoUpdate( const sword::CrowdConcentrationDestruction& message );
    void Update();
    void CreateDictionary();
    void ComputeCenter();
    QString GetAttitude() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    const unsigned int male_;
    const unsigned int female_;
    const unsigned int children_;
    kernel::OptionalValue< unsigned int > armedIndividuals_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f center_;
    kernel::OptionalValue< int > nDomination_;
    kernel::OptionalValue< int > healthy_;
    kernel::OptionalValue< int > wounded_;
    kernel::OptionalValue< int > contaminated_;
    kernel::OptionalValue< int > dead_;
    kernel::OptionalValue< QString > attitude_;
    //@}

};

#endif // __Population_h_
