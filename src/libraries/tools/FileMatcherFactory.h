// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcherFactory_h
#define FileMatcherFactory_h

#include "FileMatcherFactory_ABC.h"

namespace tools
{
// =============================================================================
/** @class  FileMatcherFactory
    @brief  FileMatcherFactory
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcherFactory : public FileMatcherFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcherFactory();
    virtual ~FileMatcherFactory();
    //@}

    //! @name Accessors
    //@{
    virtual boost::shared_ptr< FileMatcher_ABC > CreateFileMatcher( xml::xistream& xis ) const;
    //@}
};

}

#endif // FileMatcherFactory_h
