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
    //@}

private:
    //! @name Member data
    //@{
    std::string sQname_;
    double latitude_;
    double longitude_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Point_h_
