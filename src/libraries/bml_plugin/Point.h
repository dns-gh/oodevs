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

namespace plugins
{
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
             Point();
             Point( xml::xistream& xis, const std::string& fieldname );
             Point( double latitude, double longitude );
    virtual ~Point();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const std::string& fieldname ) const;
    void Serialize( ASN1T_CoordLatLong& asn ) const;
    unsigned short ComputeBearing( const Point& rhs ) const;
    //@}

private:
    //! @name Member data
    //@{
    double latitude_;
    double longitude_;
    double height_;
    //@}
};

}
}

#endif // __Point_h_
