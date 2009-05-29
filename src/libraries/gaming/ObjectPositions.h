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

#include "LocationPositions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class ObjectType;
    class Location_ABC;
}

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public LocationPositions
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                      , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
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
    //! @name Copy/Assignement
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    //@}

private:
    //! @name Member data
    //@{
    const std::string symbol_;
    //@}
};

#endif // __ObjectPositions_h_
