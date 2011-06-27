// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_Handler_ABC_h
#define tools_Handler_ABC_h

#include <boost/noncopyable.hpp>

namespace tools
{
// =============================================================================
/** @class  Handler_ABC
    @brief  Handler definition
*/
// Created: SLI 2011-06-24
// =============================================================================
class Handler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Handler_ABC() {}
    virtual ~Handler_ABC() {}
    //@}
};

}

#endif // tools_Handler_ABC_h
