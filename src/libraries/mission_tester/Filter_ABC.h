// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Filter_ABC_h_
#define __Filter_ABC_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  Filter_ABC
    @brief  Filter_ABC
*/
// Created: PHC 2011-03-31
// =============================================================================
class Filter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Filter_ABC() {}
    virtual ~Filter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Accepts( const std::string& criterion ) const = 0;
    //@}
};

}

#endif // __Filter_ABC_h_
