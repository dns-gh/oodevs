// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Note_h_
#define __Note_h_

#include "protocol/MessengerSenders.h"
#include <boost/noncopyable.hpp>
#include <list>

namespace xml
{
    class xistream;
    class xostream;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace plugins
{
namespace messenger
{
// =============================================================================
/** @class  Note
    @brief  Note
*/
// Created: HBD 2010-01-15
// =============================================================================
class Note : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Note( unsigned long id, const sword::MarkerCreationRequest& message, const std::string& currentTime );
             Note( unsigned long id, std::vector<std::string>& values, unsigned int parent, const std::string& currentTime );
    virtual ~Note();
    //@}

    //! @name Operations
    //@{
    void WriteNote( std::ostream& file, int parentLine ) const;
    unsigned long GetId() const;
    unsigned long GetParent() const;

    const std::list< unsigned long >& GetChildren() const;
    void RemoveChild( unsigned long note );
    void AddChild( unsigned long note );
    void SetParent(unsigned long note);

    void Update( const sword::MarkerUpdateRequest& message, const std::string& currentTime );

    virtual void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher, unsigned int context = 0 ) const;
    virtual void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher, bool stateParent ) const;
    virtual void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string number_;
    std::string description_;
    unsigned long parent_;
    std::list< unsigned long > children_;
    std::string creationTime_;
    std::string lastUpdateTime_;
    //@}
};

}
}

#endif // __Note_h_
