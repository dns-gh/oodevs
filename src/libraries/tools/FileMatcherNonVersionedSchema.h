// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcherNonVersionedSchema_h
#define FileMatcherNonVersionedSchema_h

#include "FileMatcher_ABC.h"

namespace tools
{
// =============================================================================
/** @class  FileMatcherNonVersionedSchema
    @brief  FileMatcherNonVersionedSchema
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcherNonVersionedSchema : public FileMatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcherNonVersionedSchema( xml::xistream& xisConfiguration );
    virtual ~FileMatcherNonVersionedSchema();
    //@}

    //! @name Accessors
    //@{
    virtual bool MatchAndReturnNewSchema( const Path& inputFileName, xml::xistream& xisInput, Path& outputSchema ) const;
    //@}

private:
    const Path nonVersionedSchemaToMatch_;
    const Path assignedSchema_;
};

}

#endif // RealFileLoader_ABC_h
