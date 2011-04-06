// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Facade_h_
#define __Facade_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  Facade
    @brief  Facade
*/
// Created: PHC 2011-04-05
// =============================================================================
class Facade : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             Facade();
    virtual ~Facade();
    //@}

    //! @name Operations
    //@{
    void Run( int argc, char* argv[] );
    //@}
};
}

#endif // __Facade_h_
