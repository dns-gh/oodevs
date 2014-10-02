// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PACKAGE_H
#define PACKAGE_H

#include "Package_ABC.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
}

namespace host
{
    typedef boost::filesystem::path Path;

// =============================================================================
/** @class  Package
    @brief  Package class definition
*/
// Created: BAX 2012-05-14
// =============================================================================
class Package : public Package_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Package( runtime::Pool_ABC& pool, const runtime::FileSystem_ABC& fs,
                      const Path& path, bool reference, bool replace );
    virtual ~Package();
    //@}

    //! @name Package_ABC methods
    //@{
    virtual Tree GetProperties() const;
    virtual Tree GetPropertiesFrom( const Item_ABC& item ) const;
    virtual size_t GetSize() const;
    virtual Path GetPath() const;
    virtual Path GetRoot( const Item_ABC& item ) const;
    virtual T_Item Find( size_t id, bool alive ) const;
    virtual T_Item Find( const Item_ABC& item, bool alive ) const;
    virtual T_Item Find( const Path& root, const std::string& checksum, bool alive ) const;
    virtual T_Item Find( const std::string& type, const std::string& name, const std::string& checksum ) const;
    virtual T_Exercises GetExercises( int offset, int limit ) const;
    virtual size_t CountExercises() const;
    virtual bool Parse();
    virtual void Identify( const Package_ABC& reference );
    virtual void InstallWith( runtime::Async& async, const Path& root, const T_Items& items, bool move );
    virtual void Install( runtime::Async& async, const Path& root, const Package_ABC& src, const std::vector< size_t >& ids );
    virtual void Uninstall( runtime::Async& async, const Path& root, const std::vector< size_t >& ids );
    virtual Tree LinkExercise( const std::string& name );
    virtual Tree LinkItem( Item_ABC& item );
    virtual Tree LinkItem( const Tree& tree );
    virtual Tree GetExerciseProperties( const std::string& name );
    virtual bool UnlinkItem( runtime::Async& async, const Tree& tree );
    virtual void Download( web::Chunker_ABC& dst, const Item_ABC& item );
    static T_Item MakeDependency( const std::string& type, const std::string& name );
    //@}

public:
    //! @name Typedef helpers
    //@{
    typedef std::vector< boost::shared_ptr< Item_ABC > > T_Items;
    //@}

private:

    //! @name Private members
    //@{
    runtime::Pool_ABC& pool_;
    const runtime::FileSystem_ABC& fs_;
    const Path path_;
    const bool reference_;
    const bool replace_;
    std::string name_;
    std::string description_;
    std::string version_;
    T_Items items_;
    //@}
};
}

#endif // PACKAGE_H
