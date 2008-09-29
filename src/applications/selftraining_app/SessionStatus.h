// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __SessionStatus_h_
#define __SessionStatus_h_

class Session ; 

// =============================================================================
/** @class  SessionStatus
    @brief  SessionStatus
*/
// Created: RDS 2008-09-29
// =============================================================================
class SessionStatus
{

public:
    
    enum Status 
    {
        GUI_OPENED, 
        GUI_CLOSED,
        SIM_STARTED, 
        SIM_AVAILABLE 
    }; 

    //! @name Constructors/Destructor
    //@{
    explicit SessionStatus( const Status status, Session& session );
    virtual ~SessionStatus();
    //@}

    Session& GetSession() const ;  
    Status GetStatus() const ; 

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionStatus( const SessionStatus& );            //!< Copy constructor
    SessionStatus& operator=( const SessionStatus& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    Session& session_ ; 
    Status   status_ ; 
    //@}
};

#endif // __SessionStatus_h_
