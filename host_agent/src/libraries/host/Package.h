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

namespace host
{
    class FileSystem_ABC;
    class Pool_ABC;
    typedef boost::filesystem3::path Path;

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
             Package( Pool_ABC& pool, const FileSystem_ABC& system, const Path& path, bool reference );
    virtual ~Package();
    //@}

    //! @name Package_ABC methods
    //@{
    virtual Tree GetProperties() const;
    virtual bool Parse();
    virtual void Identify( const Package_ABC& reference );
    virtual T_Item Find( size_t id ) const;
    virtual T_Item Find( const Item_ABC& item ) const;
    virtual void Install( const Package_ABC& src, const std::vector< size_t >& ids );
    virtual void Remove( const std::vector< size_t >& ids );
    virtual void Remove();
    //@}

public:
    //! @name Typedef helpers
    //@{
    typedef std::vector< boost::shared_ptr< Item_ABC > > T_Items;
    //@}

private:

    //! @name Private members
    //@{
    Pool_ABC& pool_;
    const FileSystem_ABC& system_;
    const Path path_;
    const bool reference_;
    std::string name_;
    std::string description_;
    std::string version_;
    T_Items items_;
    //@}
};

}

#endif // PACKAGE_H
