// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef SchemaVersionExtractor_ABC_h
#define SchemaVersionExtractor_ABC_h

#include <boost/noncopyable.hpp>

namespace tools
{
    class Path;

// =============================================================================
/** @class  SchemaVersionExtractor_ABC
    @brief  SchemaVersionExtractor_ABC
*/
// Created: NLD 2011-02-14
// =============================================================================
class SchemaVersionExtractor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SchemaVersionExtractor_ABC() {}
    virtual ~SchemaVersionExtractor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Path ExtractVersion( const Path& schema ) const = 0;
    //@}
};

}

#endif // SchemaVersionExtractor_ABC_h