// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef tools_PhyLoader_h
#define tools_PhyLoader_h

#include <tools/Path.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <memory>
#include <string>

namespace xml
{
    class xistream;
}

namespace tools
{
    class Loader_ABC;
    class ExerciseConfig;

class PhyLoader : private boost::noncopyable
{
public:
    typedef std::function< void ( xml::xistream& ) > T_Loader;

    struct File
    {
        boost::shared_ptr< const xml::xistream > xml;
        tools::Path path;
    };

public:
             PhyLoader( const Path& physicalFile, const ExerciseConfig& config,
                     const boost::shared_ptr< Loader_ABC >& loader );
    virtual ~PhyLoader();

    // Return the file path/name loaded
    Path LoadPhysicalFile( const std::string& rootTag, T_Loader loader ) const;
    // Return the file path/name loaded
    Path LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const;
    Path GetPhysicalChildFile( const std::string& rootTag ) const;
    Path GetPhysicalChildPath( const std::string& rootTag ) const;
    File GetPhysicalXml( const std::string& rootTag, bool optional ) const;

private:
    Path LoadPhysicalFile( const std::string& rootTag, T_Loader loader, bool optional ) const;

private:
    const boost::shared_ptr< Loader_ABC > loader_;
    const ExerciseConfig& config_;
    std::map< std::string, std::string > allowedFiles_;
    std::map< std::string, std::string > allowedPaths_;
    mutable std::map< std::string, boost::shared_ptr< xml::xistream > > cached_;
};

}

#endif // tools_PhyLoader_h
