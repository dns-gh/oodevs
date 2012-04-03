// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "LocationPositions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class ObjectType;
    class Location_ABC;
}

namespace sword
{
    class ObjectCreation;
    class ObjectUpdate;
}

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public LocationPositions
                      , public kernel::Updatable_ABC< sword::ObjectUpdate >
                      , public kernel::Updatable_ABC< sword::ObjectCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPositions( const kernel::ObjectType& type, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ObjectPositions();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    virtual void DoUpdate( const sword::ObjectCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ObjectType& type_;
    std::string symbol_;
    //@}
};

#endif // __ObjectPositions_h_
