// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMigration_ABC_h
#define FileMigration_ABC_h

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace tools
{
    class Path;

// =============================================================================
/** @class  FileMigration_ABC
    @brief  FileMigration_ABC
*/
// Created: NLD 2011-02-14
// =============================================================================
class FileMigration_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMigration_ABC() {}
    virtual ~FileMigration_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual const Path& GetFromVersion() const = 0;
    virtual const Path& GetToVersion  () const = 0;
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< xml::xistream > UpgradeFile( std::unique_ptr< xml::xistream > file, const Path& schema ) const = 0;
    //@}
};

}

#endif // RealFileLoader_ABC_h
