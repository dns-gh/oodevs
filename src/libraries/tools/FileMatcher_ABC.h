// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMatcher_ABC_h
#define FileMatcher_ABC_h

#include <boost/noncopyable.hpp>
#include "Path.h"

namespace xml
{
    class xistream;
}

namespace tools
{
// =============================================================================
/** @class  FileMatcher_ABC
    @brief  FileMatcher_ABC
*/
// Created: NLD 2011-03-08
// =============================================================================
class FileMatcher_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMatcher_ABC() {}
    virtual ~FileMatcher_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual bool MatchAndReturnNewSchema( const Path& inputFileName, xml::xistream& xisInput, Path& outputSchema ) const = 0;
    //@}
};

}

#endif // RealFileLoader_ABC_h
