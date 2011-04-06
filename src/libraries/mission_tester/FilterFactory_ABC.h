// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterFactory_ABC_h_
#define __FilterFactory_ABC_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace mission_tester
{
    class Filter_ABC;
// =============================================================================
/** @class  FilterFactory_ABC
    @brief  FilterFactory_ABC
*/
// Created: PHC 2011-04-06
// =============================================================================
class FilterFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             FilterFactory_ABC() {}
    virtual ~FilterFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Filter_ABC > Create( const std::string& criteria ) = 0;
    //@}
};
}

#endif // __FilterFactory_ABC_h_
