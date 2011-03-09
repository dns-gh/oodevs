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
    virtual bool MatchAndReturnNewSchema( const std::string& inputFileName, xml::xistream& xisInput, std::string& outputSchema ) const;
    //@}

private:
    const std::string fileNameToMatch_;
    const std::string assignedSchema_;
};

}

#endif // RealFileLoader_ABC_h
