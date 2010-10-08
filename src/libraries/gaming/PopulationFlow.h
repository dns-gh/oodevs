// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_

#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  PopulationFlow
    @brief  PopulationFlow
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationFlow : public kernel::PopulationFlow_ABC
                     , public kernel::Positions
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowUpdate >
                     , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const MsgsSimToClient::MsgCrowdFlowCreation& message, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual QString  GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual float GetDensity() const;
    virtual QString      GetAttitude() const;
    virtual float GetHeight( bool aggregated ) const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision, float adaptiveFactor ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;

    uint          nID_;
    T_PointVector itineraire_; // $$$$ AGE 2006-03-23: extension
    T_PointVector flow_;
    geometry::Rectangle2f boundingBox_;
    int              nDirection_;
    int              nSpeed_;

    int           nLivingHumans_;
    int           nDeadHumans_;
    float         rDensity_;
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationFlow_h_
