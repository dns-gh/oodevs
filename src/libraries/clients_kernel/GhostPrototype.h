// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostPrototype_h_
#define __GhostPrototype_h_

#include <boost/noncopyable.hpp>
#include "ENT/ENT_Enums_Gen.h"

namespace kernel
{

// =============================================================================
/** @class  GhostPrototype
    @brief  GhostPrototype
*/
// Created: ABR 2011-10-18
// =============================================================================
class GhostPrototype : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GhostPrototype() : ghostType_( eGhostType_Invalid ) {}
    virtual ~GhostPrototype() {}
    //@}

public:
    //! @name Member data
    //@{
    E_GhostType ghostType_;
    std::string name_;
    std::string type_;
    std::string symbol_;
    std::string level_;
    //@}
};

}

#endif // __GhostPrototype_h_
