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
#include <boost/optional.hpp>

namespace sword
{
    class SimToClient;
}

namespace plugins
{
namespace logistic
{

class NameResolver_ABC;

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
    ConsignResolver_ABC( const tools::Path& name, const NameResolver_ABC& nameResolver );
    virtual ~ConsignResolver_ABC();
    //@}

    //! @name Operations
    //@{
    virtual ConsignData_ABC* CreateConsignData( int requestId ) = 0;
    bool Receive( const sword::SimToClient& message, ConsignData_ABC& consign,
            const boost::gregorian::date& today );
    const NameResolver_ABC& GetNameResolver() const;
    virtual void InitHeader() = 0;
    virtual void AddToLineIndex( int number ) { curLineIndex_ += number; }
    void SetMaxLinesInFile( int maxLines ) { maxLinesInFile_ = maxLines; }
    //@}

protected:

    //! @name Operations
    //@{
    virtual boost::optional< std::string > ManageMessage(
            const sword::SimToClient& msg, ConsignData_ABC& consign ) = 0;

    template < typename M, typename T >
    std::string TraceConsign( const M& msg, ConsignData_ABC& consignData )
    {
        ConsignWriter writer;
        dynamic_cast< T& >( consignData ).ManageMessage( msg, *this ).WriteConsign( writer );
        return writer.GetLine();
    }
    //@}

    //! @name Helpers
    //@{
    void InitFileIndex( const boost::gregorian::date& today );
    void CheckOutputFile( const boost::gregorian::date& today );
    void SetNewFile( const boost::gregorian::date& today );
    void RemoveOldFiles( const boost::gregorian::date& today );
    void OpenFile();
    void SetHeader( const ConsignData_ABC& consign );
    //@}

    //! @name Member data
    //@{
    const NameResolver_ABC& nameResolver_; 
    std::string header_;
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
