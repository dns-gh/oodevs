// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Mission_h_
#define __Mission_h_

#include "Position.h"
#include <xeumeuleu/xml.hpp>
#include <string>
#include <vector>

class Mapping;

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: LDC 2010-07-09
// =============================================================================
class Mission
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission();
             Mission( const std::string& id, const std::vector< std::vector< Position > >& tacticals, Mapping& mapping );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Mission& mission );
    //@}

private:
    //! @name Member data
    //@{
    std::string id_;
    std::vector< std::vector< Position > > tacticals_;
    Mapping* mapping_;
    //@}
};

#endif // __Mission_h_
