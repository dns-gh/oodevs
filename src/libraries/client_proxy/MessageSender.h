// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MessageSender_h_
#define __MessageSender_h_

namespace sword
{
    class ClientToAuthentication;
    class ClientToSim;
    class ClientToMessenger;
}

namespace tools
{
    class MessageSender_ABC;
}

// =============================================================================
/** @class  MessageSender
    @brief  MessageSender
*/
// Created: SEB 2010-10-12
// =============================================================================
class MessageSender
{

public:
    //! @name Constructors/Destructor
    //@{
             MessageSender( tools::MessageSender_ABC& sender, const std::string& endpoint );
    virtual ~MessageSender();
    //@}

    //! @name Operations
    //@{
    void Send( const sword::ClientToAuthentication& message );
    void Send( const sword::ClientToSim& message );
    void Send( const sword::ClientToMessenger& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageSender( const MessageSender& );            //!< Copy constructor
    MessageSender& operator=( const MessageSender& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    const std::string endpoint_;
    //@}
};

#endif // __MessageSender_h_
