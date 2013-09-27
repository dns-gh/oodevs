// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_h_
#define __PopulationConcentration_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Options;
}

namespace sword
{
    class CrowdConcentrationCreation;
    class CrowdConcentrationUpdate;
}

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationConcentration : public kernel::PopulationConcentration_ABC
                              , public kernel::Positions
                              , public kernel::Updatable_ABC< sword::CrowdConcentrationUpdate >
                              , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( kernel::Options& options, const sword::CrowdConcentrationCreation& message, const kernel::CoordinateConverter_ABC& converter, float density );
    virtual ~PopulationConcentration();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetHealthyHumans() const;
    virtual unsigned int GetWoundedHumans() const;
    virtual unsigned int GetContaminatedHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual float GetRadius() const;
    virtual float GetDensity() const;
    virtual QString GetAttitude() const;
    virtual float GetHeight( bool aggregated ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::CrowdConcentrationUpdate& message );
    void SelectRightPartColor() const;
    QColor GetColor( const std::string& option, const QColor& defaultColor ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Options& options_;
    geometry::Point2f position_;
    geometry::Rectangle2f boundingBox_;
    float density_;
    unsigned int nID_;
    int nHealthyHumans_;
    int nWoundedHumans_;
    int nContaminatedHumans_;
    int nDeadHumans_;
    float radius_;
    float deadRadius_;
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationConcentration_h_
