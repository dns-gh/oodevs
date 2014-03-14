// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __PopulationPartPositionsProxy_h_
#define __PopulationPartPositionsProxy_h_

#include "clients_kernel/Positions.h"

// =============================================================================
/** @class  PopulationPartPositionsProxy
    @brief  PopulationPartPositionsProxy
*/
// Created: SBO 2008-08-12
// =============================================================================
class PopulationPartPositionsProxy : public kernel::Positions
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationPartPositionsProxy( const kernel::Positions& positions );
    virtual ~PopulationPartPositionsProxy();
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
    PopulationPartPositionsProxy( const PopulationPartPositionsProxy& );            //!< Copy constructor
    PopulationPartPositionsProxy& operator=( const PopulationPartPositionsProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Positions& positions_;
    //@}
};

#endif // __PopulationPartPositionsProxy_h_
