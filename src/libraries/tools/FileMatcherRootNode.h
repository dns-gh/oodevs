// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcherRootNode_h
#define FileMatcherRootNode_h

#include "FileMatcher_ABC.h"

namespace tools
{
// =============================================================================
/** @class  FileMatcherRootNode
    @brief  FileMatcherRootNode
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcherRootNode : public FileMatcher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcherRootNode( xml::xistream& xisConfiguration );
    virtual ~FileMatcherRootNode();
    //@}

    //! @name Accessors
    //@{
    virtual bool MatchAndReturnNewSchema( const std::string& inputFileName, xml::xistream& xisInput, std::string& outputSchema ) const;
    //@}

private:
    const std::string rootNodeToMatch_;
    const std::string assignedSchema_;
};

}

#endif // RealFileLoader_ABC_h
