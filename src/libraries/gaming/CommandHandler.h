// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandHandler_h_
#define __CommandHandler_h_

class CommandHandler_ABC;

// =============================================================================
/** @class  CommandHandler
    @brief  CommandHandler
*/
// Created: AGE 2008-06-12
// =============================================================================
class CommandHandler
{

public:
    //! @name Constructors/Destructor
    //@{
             CommandHandler();
    virtual ~CommandHandler();
    //@}

    //! @name Operations
    //@{
    void Register  ( const std::string& command, CommandHandler_ABC& handler );
    void Unregister( const std::string& command, CommandHandler_ABC& handler );

    void Receive( const std::string& from, const std::string& to, const std::string& command );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CommandHandler( const CommandHandler& );            //!< Copy constructor
    CommandHandler& operator=( const CommandHandler& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::multimap< std::string, CommandHandler_ABC* >   T_Handlers;
    typedef T_Handlers::value_type                              T_NamedHandler;
    typedef T_Handlers::iterator                               IT_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    T_Handlers handlers_;
    //@}
};

#endif // __CommandHandler_h_
