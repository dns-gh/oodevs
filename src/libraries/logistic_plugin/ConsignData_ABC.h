// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsignData_ABC_h_
#define __ConsignData_ABC_h_

#include "ConsignWriter.h"
#include "LogisticPlugin.h"
#include "tools/FileWrapper.h"
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

namespace plugins
{
namespace logistic
{
    class ConsignWriter;

// =============================================================================
/** @class  ConsignData_ABC
    @brief  ConsignData_ABC
*/
// Created: MMC 2012-08-06
// =============================================================================
class ConsignData_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ConsignData_ABC( LogisticPlugin::E_LogisticType type, const std::string& requestId )
                : type_( type )
                , requestId_( requestId ) {}
    virtual ~ConsignData_ABC() {}
    //@}

    //! @name Operations
    //@{
    LogisticPlugin::E_LogisticType GetType() const
    {
        return type_;
    }

    void SetTime( int tick, const std::string& time )
    {
        tick_ = boost::lexical_cast< std::string >( tick );
        simTime_ = time;
    }

    int GetTick() const
    {
        if( tick_.empty() )
            return 0;
        return std::atoi( tick_.c_str() );
    }

    std::string ToString() const
    {
        ConsignWriter w;
        WriteConsign( w );
        return w.GetLine();
    }
    //@}

private:
    virtual void WriteConsign( ConsignWriter& w ) const = 0;

public:
    //! @name Member data
    //@{
    std::string tick_;
    std::string simTime_;
    //@}

protected:
    //! @name Member data
    //@{
    const LogisticPlugin::E_LogisticType type_;
    std::string requestId_;
    //@}
};

}
}

#endif // __ConsignData_ABC_h_
