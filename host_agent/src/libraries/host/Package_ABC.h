// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PACKAGE_ABC_H
#define PACKAGE_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

namespace host
{

// =============================================================================
/** @class  Package_ABC
    @brief  Package_ABC interface
*/
// Created: BAX 2012-05-14
// =============================================================================
class Package_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Package_ABC() {}
    virtual ~Package_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::property_tree::ptree GetProperties() const = 0;
    virtual bool Parse() = 0;
    //@}
};

}

#endif // PACKAGE_ABC_H
