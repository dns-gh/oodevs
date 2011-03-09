// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcherFactory_ABC_h
#define FileMatcherFactory_ABC_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace tools
{
    class FileMatcher_ABC;

// =============================================================================
/** @class  FileMatcherFactory_ABC
    @brief  FileMatcherFactory_ABC
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcherFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcherFactory_ABC() {};
    virtual ~FileMatcherFactory_ABC() {};
    //@}

    //! @name Accessors
    //@{
    virtual boost::shared_ptr< FileMatcher_ABC > CreateFileMatcher( xml::xistream& xis ) const = 0;
    //@}
};

}

#endif // FileMatcherFactory_ABC_h
