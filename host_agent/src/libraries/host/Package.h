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

namespace host
{
    class FileSystem_ABC;

// =============================================================================
/** @class  SubPackage
    @brief  SubPackage class definition
*/
// Created: BAX 2012-05-14
// =============================================================================
struct SubPackage
{
     SubPackage( const boost::filesystem::path& root );
    ~SubPackage();
    const std::string name_;
    const std::string date_;
    const std::string checksum_;
    const std::string action_;
};

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
    //! @name Private methods
    //@{
    void ParseModels();
    void ParseTerrains();
    void ParseExercises();
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< SubPackage > T_Packages;
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
    T_Packages models;
    T_Packages terrains;
    T_Packages exercises;
    //@}
};

}

#endif // PACKAGE_H
