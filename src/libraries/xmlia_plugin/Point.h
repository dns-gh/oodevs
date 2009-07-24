// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XMLIA_Point_h_
#define __XMLIA_Point_h_

#include "Entity_ABC.h"

struct ASN1T_CoordLatLong;

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  Point
    @brief  Point
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Point : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Point();
    Point( xml::xistream& xis );
    Point( const double& latitude, const double& longitude );
    virtual ~Point();
    //@}

    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Update( dispatcher::Agent& agent );
    void FillLatLong( ASN1T_CoordLatLong& utm ) const;
    //@}

private:
    //! @name Member data
    //@{
    double latitude_;
    double longitude_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Point_h_
