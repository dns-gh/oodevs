// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Object_h_
#define __Object_h_

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
    virtual ~Object();
    //@}

    //! @name Operations
    //@{
    void Read( xml::xistream& xis, const std::string& id, Mapping& mapping );
    void Write( xml::xostream& xos ) const;
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

#endif // __Object_h_
