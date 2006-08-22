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

#include "ASN_Types.h"
#include "PopulationPart_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class PopulationFlow : public PopulationPart_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg, const kernel::CoordinateConverter_ABC& converter );
    virtual ~PopulationFlow();
    //@}

    //! @name Operations
    //@{
    virtual std::string  GetName() const;
    virtual unsigned long GetId() const;
    virtual unsigned int GetLivingHumans() const;
    virtual unsigned int GetDeadHumans() const;
    virtual unsigned int GetDensity() const;

    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual geometry::Point2f GetPosition() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}

    //! @name Accessors
    //@{
//	virtual const std::string&   GetName            () const;
//    virtual const MT_Vector2D&   GetPos             () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
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
    //@}
};

#endif // __PopulationFlow_h_
