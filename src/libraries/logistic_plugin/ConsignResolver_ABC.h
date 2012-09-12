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

class NameResolver_ABC;

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
    ConsignResolver_ABC( const std::string& name, const NameResolver_ABC& nameResolver );
    virtual ~ConsignResolver_ABC();
    //@}

    //! @name Operations
    //@{
    bool Receive( const sword::SimToClient& message, const boost::gregorian::date& today );
    void SetTime( int tick, const std::string& simTime );
    void GetSimTime( std::string& simTime, std::string& tick ) const;
    const NameResolver_ABC& GetNameResolver() const;
    virtual void InitHeader() = 0;
    virtual void AddToLineIndex( int number ) { curLineIndex_ += number; }
    void SetMaxLinesInFile( int maxLines ) { maxLinesInFile_ = maxLines; }
    int GetConsignCount() const;
    //@}

protected:

    //! @name Operations
    //@{
    virtual bool IsManageable( const sword::SimToClient& ) { return false; }
    virtual void ManageMessage( const sword::SimToClient& ) {}
    virtual bool IsEmptyLineMessage( const sword::SimToClient& ) = 0;
    ConsignData_ABC& ConsignResolver_ABC::GetConsign( int requestId );
    virtual ConsignData_ABC* CreateConsignData( int requestId ) = 0;
    virtual void DestroyConsignData( int requestId );

    template < typename M, typename T >
    void TraceConsign( const M& msg, std::ofstream& output )
    {
        if( msg.has_request() )
        {
            static_cast< T& >( GetConsign( static_cast< int >( msg.request().id() ) ) ).ManageMessage( msg, *this ) >> output;
            output.flush();
        }
    }
    //@}

    //! @name Helpers
    //@{
    void AppendDateWithExtension( std::string& fileName, const boost::gregorian::date& d, int fileIndex );
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
    int curTick_;
    std::string simTime_;
    const std::string name_;
    std::string fileName_;
    std::ofstream output_;
    std::map< int, ConsignData_ABC* > consignsData_;
    int curLineIndex_;
    int maxLinesInFile_;
    //@}
};
}
}

#endif // __ConsignResolver_ABC_h_
