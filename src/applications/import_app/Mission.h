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
#include <string>
#include <vector>
#include <xeumeuleu/xml.hpp>
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
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    void Set( const std::string& missionId, const std::vector< std::vector< Position > >& tacticals );
    void Write( xml::xostream& xos, Mapping& mapping );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::vector< Position > > tacticals_;
    std::string id_;
    //@}
};

#endif // __Mission_h_
