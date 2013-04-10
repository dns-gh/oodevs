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
namespace filesystem
{
    class path;
}
}

namespace runtime
{
    struct Async;
}

namespace web
{
    struct Chunker_ABC;
}

namespace host
{
    typedef boost::filesystem::path Path;
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
    typedef std::vector< Path >           T_Exercises;
    typedef std::vector< T_Item >         T_Items;
    //@}

    //! @name Methods
    //@{
    virtual Tree GetProperties() const = 0;
    virtual Tree GetPropertiesFrom( const Item_ABC& item ) const = 0;
    virtual size_t GetSize() const = 0;
    virtual Path GetPath() const = 0;
    virtual Path GetRoot( const Item_ABC& item ) const = 0;
    virtual T_Item Find( size_t id, bool alive ) const = 0;
    virtual T_Item Find( const Item_ABC& item, bool alive ) const = 0;
    virtual T_Item Find( const std::string& type, const std::string& name, const std::string& checksum ) const = 0;
    virtual T_Exercises GetExercises( int offset, int limit ) const = 0;
    virtual size_t CountExercises() const = 0;
    virtual bool Parse() = 0;
    virtual void Identify( const Package_ABC& ref ) = 0;
    virtual void InstallWith( runtime::Async& async, const Path& root, const T_Items& items, bool move ) = 0;
    virtual void Install( runtime::Async& async, const Path& root, const Package_ABC& src, const std::vector< size_t >& ids ) = 0;
    virtual void Uninstall( runtime::Async& async, const Path& root, const std::vector< size_t >& ids ) = 0;
    virtual Tree LinkExercise( const std::string& name ) = 0;
    virtual Tree LinkItem( Item_ABC& item ) = 0;
    virtual Tree LinkItem( const Tree& tree ) = 0;
    virtual Tree GetExerciseProperties( const std::string& name ) = 0;
    virtual bool UnlinkItem( runtime::Async& async, const Tree& tree ) = 0;
    virtual void Download( web::Chunker_ABC& dst, const Item_ABC& item ) = 0;
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
