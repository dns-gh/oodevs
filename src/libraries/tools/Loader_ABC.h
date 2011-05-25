// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Loader_ABC_h__
#define __Loader_ABC_h__

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <string>
#include <memory>

namespace xml
{
    class xistream;
}

namespace tools
{

// =============================================================================
/** @class  Loader_ABC
    @brief  Loader definition
*/
// Created: NLD 2011-02-23
// =============================================================================
class Loader_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef boost::function< void ( xml::xistream& ) > T_Loader;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Loader_ABC() {}
    virtual ~Loader_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void                           CheckFile        ( const std::string& file ) const = 0;
    virtual void                           CheckOptionalFile( const std::string& file ) const = 0;
    virtual void                           LoadFile         ( const std::string& fileName, T_Loader loader ) const = 0;
    virtual std::auto_ptr< xml::xistream > LoadFile         ( const std::string& fileName ) const = 0;

    virtual std::string                    LoadPhysicalFile        ( const std::string& rootTag, T_Loader loader ) const = 0; // Return the file path/name loaded
    virtual std::string                    LoadOptionalPhysicalFile( const std::string& rootTag, T_Loader loader ) const = 0; // Return the file path/name loaded
    //@}
};

}

#endif // __Loader_ABC_h__
