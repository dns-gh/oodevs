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

#include "game_asn/Simulation.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"

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
                     , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowUpdate >
                     , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const ASN1T_MsgPopulationFlowCreation& asnMsg, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual QString  GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetDensity() const;
    virtual QString      GetAttitude() const;
    virtual float GetHeight() const;

    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual geometry::Point2f GetPosition() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowUpdate& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;

    uint          nID_;
	T_PointVector itineraire_; // $$$$ AGE 2006-03-23: extension
	T_PointVector flow_;
    geometry::Rectangle2f boundingBox_;
	int			  nDirection_;
	int			  nSpeed_;
    
    int           nLivingHumans_;
    int           nDeadHumans_;
    float         rDensity_;
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationFlow_h_
