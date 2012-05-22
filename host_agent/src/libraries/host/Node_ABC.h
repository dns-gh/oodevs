// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef NODE_ABC_H
#define NODE_ABC_H

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/uuid/uuid.hpp>

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
// =============================================================================
/** @class  Node_ABC
    @brief  Node_ABC interface
*/
// Created: BAX 2012-04-03
// =============================================================================
class Node_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Node_ABC() {}
    virtual ~Node_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual boost::uuids::uuid GetId() const = 0;
    virtual boost::property_tree::ptree GetProperties() const = 0;
    virtual void ReadPack( const FileSystem_ABC& system, const boost::filesystem3::path& path, std::istream& src ) = 0;
    virtual void ParsePack( const FileSystem_ABC& system, const boost::filesystem3::path& path ) = 0;
    virtual boost::property_tree::ptree GetPack() const = 0;
    //@}
};

}

#endif // NODE_ABC_H
