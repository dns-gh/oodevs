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
    struct SubPackage;

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
             Package( const FileSystem_ABC& system, const boost::filesystem::path& path );
    virtual ~Package();
    //@}

    //! @name Package_ABC methods
    //@{
    virtual boost::property_tree::ptree GetProperties() const;
    virtual bool Parse();
    //@}

private:
    //! @name Typedef helpers
    //@{
    typedef std::vector< boost::shared_ptr< SubPackage > > T_Packages;
    //@}

private:

    //! @name Private members
    //@{
    const FileSystem_ABC& system_;
    const boost::filesystem::path path_;
    bool valid_;
    std::string name_;
    std::string description_;
    std::string version_;
    T_Packages items_;
    //@}
};

}

#endif // PACKAGE_H
