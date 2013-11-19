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

#include "ConsignState.h"
#include "LogisticPlugin.h"
#include <boost/noncopyable.hpp>
#include <deque>

namespace plugins
{
namespace logistic
{
    class ConsignWriter;
    class NameResolver_ABC;

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
             ConsignData_ABC( LogisticPlugin::E_LogisticType type, const std::string& requestId );
    virtual ~ConsignData_ABC();
    //@}

    // Returns true if the consign was updated and a log entry should be added.
    bool UpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& names,
           int tick, const std::string& time );

    //! @name Operations
    //@{
    LogisticPlugin::E_LogisticType GetType() const;
    int GetTick() const;
    std::string ToString() const;
    const std::deque< ConsignState >& GetHistory() const;
    //@}

protected:
    ConsignState& PushState();

private:
    virtual bool DoUpdateConsign( const sword::SimToClient& msg, const NameResolver_ABC& names ) = 0;
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

private:
    std::deque< ConsignState > history_;
};

}
}

#endif // __ConsignData_ABC_h_
