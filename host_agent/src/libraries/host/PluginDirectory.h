// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PLUGIN_DIRECTORY_H
#define PLUGIN_DIRECTORY_H

#include <boost/filesystem/path.hpp>
#include <map>
#include <vector>

namespace runtime
{
    struct FileSystem_ABC;
}

namespace host
{
    struct Plugin;
    typedef boost::filesystem::path Path;
}

namespace host
{
// =============================================================================
/** @class  PluginDirectory
    @brief  PluginDirectory class
*/
// Created: BAX 2012-08-24
// =============================================================================
class PluginDirectory
{
public:
    //! @name Constructors/Destructor
    //@{
     PluginDirectory( const runtime::FileSystem_ABC& fs,
                      const Path& root );
    ~PluginDirectory();
    //@}

    //! @name Typedef helpers
    //@{
    typedef std::vector< Path >      T_Names;
    typedef std::map< Path, Plugin > T_Plugins;
    //@}

    //! @name Methods
    //@{
    size_t  Count   () const;
    T_Names GetNames( int offset, int limit ) const;
    bool    Has     ( const std::string& name ) const;
    //@}

private:
    T_Plugins plugins_;
};
}

#endif // PLUGIN_DIRECTORY_H
