// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XMLIA_NBCObject_h_
#define __XMLIA_NBCObject_h_

#include "Entity_ABC.h"

namespace dispatcher
{
  class Object;
}

namespace plugins
{
namespace xmlia
{
  class Point;
// =============================================================================
/** @class  NBCObject
    @brief  NBCObject
*/
// Created: MGD 2009-06-12
// =============================================================================
  class NBCObject : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    NBCObject( xml::xistream& xis );
    NBCObject( dispatcher::Object& obj );
    virtual ~NBCObject();
    //@}

    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Update( dispatcher::Object& obj );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string type_;
    Point* localisation_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_NBCObject_h_
