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
    class Path;

    class Loader_ABC;
    class ExerciseConfig;

// =============================================================================
/** @class  PhyLoader
    @brief  PhyLoader
*/
// Created: NLD 2011-02-14
// =============================================================================
class PhyLoader : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::function< void ( xml::xistream& ) > T_Loader;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PhyLoader( const Path& physicalFile, const ExerciseConfig& config,
                     const boost::shared_ptr< Loader_ABC >& loader );
    virtual ~PhyLoader();
    //@}

    //! @name Operations
    //@{
    virtual Path LoadPhysicalFile        ( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    virtual Path LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const; // Return the file path/name loaded
    virtual Path GetPhysicalChildFile( const std::string& rootTag ) const;
    virtual Path GetPhysicalChildPath( const std::string& rootTag ) const;
    //@}

private:
    Path LoadPhysicalFile( const std::string& rootTag, T_Loader loader, bool optional ) const;

private:
    //! @name Member data
    //@{
    const boost::shared_ptr< Loader_ABC > loader_;
    const ExerciseConfig& config_;
    std::map< std::string, std::string > allowedFiles_;
    std::map< std::string, std::string > allowedPaths_;
    mutable std::map< std::string, boost::shared_ptr< xml::xistream > > cached_;
    //@}
};

}

#endif // tools_PhyLoader_h
