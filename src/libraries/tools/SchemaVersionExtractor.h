// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef SchemaVersionExtractor_h
#define SchemaVersionExtractor_h

#include "SchemaVersionExtractor_ABC.h"
#include <boost/regex.hpp>

namespace tools
{
// =============================================================================
/** @class  SchemaVersionExtractor
    @brief  SchemaVersionExtractor
*/
// Created: NLD 2011-02-14
// =============================================================================
class SchemaVersionExtractor : public SchemaVersionExtractor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SchemaVersionExtractor();
    virtual ~SchemaVersionExtractor();
    //@}

    //! @name Operations
    //@{
    virtual std::string ExtractVersion( const std::string& schema ) const;
    //@}

private:
    boost::regex regex_;
};

}

#endif // SchemaVersionExtractor_h
