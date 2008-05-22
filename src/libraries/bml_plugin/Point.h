// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Point_h_
#define __Point_h_

#include "game_asn/Simulation.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace bml
{

// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: SBO 2008-05-22
// =============================================================================
class Point
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Point( xml::xistream& xis );
             Point( double latitude, double longitude );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Serialize( ASN1T_CoordLatLong& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    double latitude_;
    double longitude_;
    double height_;
    std::string type_;
    //@}
};

}

#endif // __Point_h_
