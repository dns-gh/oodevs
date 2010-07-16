// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Object_h
#define import_Object_h

#include "Position.h"
#include <xeumeuleu/xml.hpp>
#include <vector>

class Mapping;

// =============================================================================
/** @class  Object
    @brief  Object
*/
// Created: LDC 2010-07-07
// =============================================================================
class Object
{
public:
    //! @name Constructors/Destructor
    //@{
             Object();
             Object( xml::xistream& xis, const std::string& id, Mapping& mapping );
    virtual ~Object();
    //@}

    //! @name Operators
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Object& object );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPositions( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    std::vector< Position > positions_;
    unsigned int objectId_;
    //@}
};

#endif // import_Object_h
