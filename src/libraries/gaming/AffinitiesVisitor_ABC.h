// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AffinitiesVisitor_ABC_h_
#define __AffinitiesVisitor_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

// =============================================================================
/** @class  AffinitiesVisitor_ABC
    @brief  Affinities visitor declaration
*/
// Created: LGY 2011-05-06
// =============================================================================
class AffinitiesVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AffinitiesVisitor_ABC() {}
    virtual ~AffinitiesVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( unsigned long id,const std::string& team, float& affinity ) = 0;
    //@}
};

#endif // __AffinitiesVisitor_ABC_h_
