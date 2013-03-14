// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcherFileName_h
#define FileMatcherFileName_h

#include "FileMatcher_ABC.h"

namespace tools
{
// =============================================================================
/** @class  FileMatcherFileName
    @brief  FileMatcherFileName
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcherFileName : public FileMatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcherFileName( xml::xistream& xisConfiguration );
    virtual ~FileMatcherFileName();
    //@}

    //! @name Accessors
    //@{
    virtual bool MatchAndReturnNewSchema( const Path& inputFileName, xml::xistream& xisInput, Path& outputSchema ) const;
    //@}

private:
    const Path fileNameToMatch_;
    const Path assignedSchema_;
};

}

#endif // RealFileLoader_ABC_h
