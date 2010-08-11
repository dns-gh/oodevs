// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_h_
#define __PopulationConcentration_h_

#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationConcentration : public kernel::PopulationConcentration_ABC
                              , public kernel::Positions
                              , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationUpdate >
                              , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration( const MsgsSimToClient::MsgPopulationConcentrationCreation& message, const kernel::CoordinateConverter_ABC& converter, float density );
    virtual ~PopulationConcentration();
    //@}

    //! @name Accessors
    //@{
    float   GetArea   () const;

    virtual QString GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual float GetDensity() const;
    virtual QString  GetAttitude() const;
    virtual float GetHeight() const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual geometry::Point2f GetPosition() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationConcentration( const PopulationConcentration& );            //!< Copy constructor
    PopulationConcentration& operator=( const PopulationConcentration& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f position_;
    geometry::Rectangle2f boundingBox_;

    float density_;
    uint  nID_;
    int   nLivingHumans_;
    int   nDeadHumans_;
    float radius_;
    float deadRadius_;
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationConcentration_h_
