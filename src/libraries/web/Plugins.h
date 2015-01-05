// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PLUGINS_H__
#define PLUGINS_H__

#include <boost/filesystem/path.hpp>
#include <map>
#include <vector>

namespace runtime
{
    struct FileSystem_ABC;
}

namespace web
{
    struct Plugin;
}

namespace tools
{
    class Path;
}

namespace web
{
// =============================================================================
/** @class  Plugins
    @brief  Plugins class
*/
// Created: BAX 2012-08-24
// =============================================================================
class Plugins
{
public:
    //! @name Constructors/Destructor
    //@{
     Plugins( const runtime::FileSystem_ABC& fs, const tools::Path& root );
    ~Plugins();
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< tools::Path >           T_Names;
    typedef std::map< tools::Path, Plugin >      T_Plugins;
    typedef std::map< std::string, std::string > T_Defaults;
    //@}

    //! @name Methods
    //@{
    size_t       Count      () const;
    T_Names      GetNames   ( int offset, int limit ) const;
    bool         Has        ( const std::string& name ) const;
    T_Defaults   GetDefaults( const tools::Path& plugin ) const;
    tools:: Path GetLibrary ( const tools::Path& plugin ) const;
    //@}

private:
    T_Plugins plugins_;
};
}

#endif // PLUGINS_H__
