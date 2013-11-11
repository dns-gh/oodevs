// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsignResolver_ABC_h_
#define __ConsignResolver_ABC_h_

#include "ConsignData_ABC.h"
#include "protocol/Protocol.h"
#include "tools/FileWrapper.h"
#include <tools/Path.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/noncopyable.hpp>

namespace sword
{
    class SimToClient;
}

namespace plugins
{
namespace logistic
{

// Return a version of string s escaped so that its characters are interpreted
// as-is when used to build a regular expression.
std::wstring EscapeRegex( const std::wstring& s );

// =============================================================================
/** @class  ConsignResolver_ABC
    @brief  ConsignResolver_ABC
*/
// Treats specialized logistic messages and traces and keeps consigns data
// Created: MMC 2012-08-06
// =============================================================================
class ConsignResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
    ConsignResolver_ABC( const tools::Path& name, const std::string& header );
    virtual ~ConsignResolver_ABC();
    //@}

    //! @name Operations
    //@{
    void Write( const std::string& data, const boost::gregorian::date& today );
    void SetMaxLinesInFile( int maxLines ) { maxLinesInFile_ = maxLines; }
    //@}

protected:
    //! @name Helpers
    //@{
    void InitFileIndex( const boost::gregorian::date& today );
    void CheckOutputFile( const boost::gregorian::date& today );
    void SetNewFile( const boost::gregorian::date& today );
    void RemoveOldFiles( const boost::gregorian::date& today );
    void OpenFile();
    //@}

    //! @name Member data
    //@{
    const std::string header_;
    boost::gregorian::date fileDate_;
    const tools::Path name_;
    tools::Path fileName_;
    int curFileIndex_;
    int curLineIndex_;
    int maxLinesInFile_;
    const int daysBeforeToKeep_;
    //@}

private:
    tools::Ofstream output_;
};
}
}

#endif // __ConsignResolver_ABC_h_
