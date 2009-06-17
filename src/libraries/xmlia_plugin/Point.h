// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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
    Point( xml::xistream& xis );
    Point( const double& latitude, const double& longitude,  unsigned idParent );
    virtual ~Point();
    //@}

    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const std::string& sQnameRapport ) const;
    virtual std::string QName() const;
    void Update( dispatcher::Agent& agent );
    void FillLatLong( ASN1T_CoordLatLong& utm );
    unsigned int GetId() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string sQname_;
    double latitude_;
    double longitude_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Point_h_
