// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectDetections_h_
#define __ObjectDetections_h_

#include "DIN_Types.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
#include "astec_kernel/Resolver_ABC.h"
#include "astec_kernel/Drawable_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  ObjectDetections
    @brief  ObjectDetections
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectDetections : public Extension_ABC
                       , public Updatable_ABC< ObjectDetectionMessage >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectDetections( Controller& controller, const Resolver_ABC< Object_ABC >& resolver );
    virtual ~ObjectDetections();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectDetections( const ObjectDetections& );            //!< Copy constructor
    ObjectDetections& operator=( const ObjectDetections& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ObjectDetectionMessage& message );
    //@}

    //! @name Types
    //@{
    typedef std::set< const Object_ABC* >   T_Objects;
    typedef T_Objects::const_iterator CIT_Objects;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Object_ABC >& resolver_;

    T_Objects perceivedObjects_;
    //@}
};

#endif // __ObjectDetections_h_
