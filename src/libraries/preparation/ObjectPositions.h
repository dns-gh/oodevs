// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "clients_kernel/Positions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public kernel::Positions
                      , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
    virtual ~ObjectPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void DoSerialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const kernel::Location_ABC* location_;
    geometry::Rectangle2f boundingBox_;
    geometry::Point2f center_;
    //@}

};

#endif // __ObjectPositions_h_
