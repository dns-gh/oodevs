// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MSEllipse_h_
#define __MSEllipse_h_

#include "game_asn/Asn.h"

namespace kernel
{
    class GlTools_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  MSEllipse
    @brief  MSEllipse
*/
// Created: AGE 2006-04-04
// =============================================================================
class MSEllipse
{

public:
    //! @name Constructors/Destructor
    //@{
             MSEllipse( const ASN1T_Location& localisation, const kernel::CoordinateConverter_ABC& converter );
    virtual ~MSEllipse();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MSEllipse( const MSEllipse& );            //!< Copy constructor
    MSEllipse& operator=( const MSEllipse& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point2f origin_;
    float minorAxis_;
    float majorAxis_;
    float angle_;
    //@}
};

#endif // __MSEllipse_h_
