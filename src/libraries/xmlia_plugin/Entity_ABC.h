// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XMLIA_Entity_ABC_h_
#define __XMLIA_Entity_ABC_h_

namespace dispatcher
{
  class Agent;
}

namespace xml
{
  class xistream;
  class xostream;
}

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: MGD 2009-06-12
// =============================================================================
class Entity_ABC
{

public:
    ///! @name Operations / Destructor
    //@{
    virtual void Serialize( xml::xostream& xos ) const = 0;
    //@}

    //! @name Destructor
    //@{
public:
  virtual ~Entity_ABC(){};

protected:
    //! @name Constructors
    //@{
    Entity_ABC(){};
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Entity_ABC_h_
