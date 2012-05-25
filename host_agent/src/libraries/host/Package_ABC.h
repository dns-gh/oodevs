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
#include <vector>

namespace boost
{
namespace filesystem3
{
    class path;
}
}

namespace host
{
    class FileSystem_ABC;
    typedef boost::filesystem3::path Path;
    typedef boost::property_tree::ptree Tree;

// =============================================================================
/** @class  Package_ABC
    @brief  Package_ABC interface
*/
// Created: BAX 2012-05-14
// =============================================================================
struct Package_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             Package_ABC() {}
    virtual ~Package_ABC() {}
    //@}

    //! @name Opaque Item
    //@{
    struct Item_ABC;
    typedef boost::shared_ptr< Item_ABC > T_Item;
    //@}

    //! @name Methods
    //@{
    virtual Tree GetProperties() const = 0;
    virtual bool Parse() = 0;
    virtual void Identify( const Package_ABC& ref ) = 0;
    virtual T_Item Find( size_t id ) const = 0;
    virtual T_Item Find( const Item_ABC& item ) const = 0;
    virtual void Install( const Package_ABC& src, const std::vector< size_t >& ids ) = 0;
    virtual void Remove( const std::vector< size_t >& ids ) = 0;
    //@}
};

// =============================================================================
/** @class  PackageFactory_ABC
    @brief  PackageFactory_ABC interface
*/
// Created: BAX 2012-05-23
// =============================================================================
struct PackageFactory_ABC : public boost::noncopyable
{
    //! @name Constructors/Destructor
    //@{
             PackageFactory_ABC() {}
    virtual ~PackageFactory_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::shared_ptr< Package_ABC > Make( const Path& path, bool reference ) const = 0;
    //@}
};

}

#endif // PACKAGE_ABC_H
