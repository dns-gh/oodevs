// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantPositions_h_
#define __InhabitantPositions_h_

#include "clients_kernel/Positions.h"

class Inhabitant;

// =============================================================================
/** @class  InhabitantPositions
    @brief  Inhabitant positions
*/
// Created: SLG 2010-11-30
// =============================================================================
class InhabitantPositions : public kernel::Positions
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit InhabitantPositions( const Inhabitant& inhabitant ); // $$$$ AGE 2006-04-10:
    virtual ~InhabitantPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual bool CanAggregate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantPositions( const InhabitantPositions& );            //!< Copy constructor
    InhabitantPositions& operator=( const InhabitantPositions& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Inhabitant& inhabitant_;
    //@}
};

#endif // __InhabitantPositions_h_
