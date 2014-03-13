// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Positions.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace sword
{
    class CrowdFlowCreation;
    class CrowdFlowUpdate;
}

// =============================================================================
/** @class  PopulationFlow
    @brief  PopulationFlow
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationFlow : public kernel::PopulationFlow_ABC
                     , public kernel::Positions
                     , public kernel::Updatable_ABC< sword::CrowdFlowUpdate >
                     , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const sword::CrowdFlowCreation& message, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetHealthyHumans() const;
    virtual unsigned int GetWoundedHumans() const;
    virtual unsigned int GetContaminatedHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual float GetDensity() const;
    virtual QString GetAttitude() const;
    virtual float GetHeight( bool aggregated ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    geometry::Point2f GetNearestPosition( const geometry::Point2f& position ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::CrowdFlowUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned int nID_;
    T_PointVector flow_;
    T_PointVector path_;
    geometry::Rectangle2f boundingBox_;
    int nDirection_;
    int nSpeed_;
    int nHealthyHumans_;
    int nWoundedHumans_;
    int nContaminatedHumans_;
    int nDeadHumans_;
    float rDensity_;
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationFlow_h_
