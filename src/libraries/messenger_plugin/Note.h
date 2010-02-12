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

#include "game_asn/Messenger.h"
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
// =============================================================================
/** @class  Note
    @brief  Note
*/
// Created: HBD 2010-01-15
// =============================================================================

namespace plugins
{
    namespace messenger 
    {

class Note
{

public:
    //! @name Constructors/Destructor
    //@{
             Note(unsigned long id, const ASN1T_MsgNoteCreationRequest& message );
    virtual ~Note();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    unsigned long GetParent() const;
    std::list<unsigned long>& GetChildren() const;
    void RemoveChild( unsigned long note );
    void AddChild( unsigned long note );
    void SetParent(unsigned long note);

    void Update( const ASN1T_MsgNoteUpdateRequest& asn );
    virtual void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher, bool stateParent ) const;
    virtual void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Note( const Note& );            //!< Copy constructor
    Note& operator=( const Note& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}
private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string number_;
    std::string description_;
    unsigned long parent_;
    std::list<unsigned long>* children_;
    //@}
};

    }
}


#endif // __Note_h_
