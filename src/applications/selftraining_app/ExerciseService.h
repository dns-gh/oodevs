// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseService_h_
#define __ExerciseService_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace frontend
{
    class Process_ABC;
}

namespace kernel
{
    class Controllers;
}

class Config;

// =============================================================================
/** @class  ExerciseService
    @brief  ExerciseService
*/
// Created: LDC 2008-10-23
// =============================================================================
class ExerciseService : public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< frontend::Process_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseService( kernel::Controllers& controllers, const Config& config );
    virtual ~ExerciseService();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const frontend::Process_ABC& process );
    virtual void NotifyUpdated( const frontend::Process_ABC& process );
    virtual void NotifyDeleted( const frontend::Process_ABC& process );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseService( const ExerciseService& );            //!< Copy constructor
    ExerciseService& operator=( const ExerciseService& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetupNetwork();
    void OnReceive( const boost::system::error_code& error, size_t bytes_received );
    void OnSendExercisesRequest( const boost::system::error_code& error );
    void RunNetwork();
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_ExercicePortList;
    typedef T_ExercicePortList::const_iterator    CIT_ExercicePortList;;
    //@}

private:
    //! @name Member data
    //@{
    T_ExercicePortList             exerciseList_;
    kernel::Controllers&           controllers_;
    const Config&                  config_;
    boost::asio::io_service        network_;
    unsigned short                 port_;
    boost::asio::ip::udp::socket   socket_;
    char                           answer_[32];
    boost::asio::ip::udp::endpoint remoteEndPoint_;
    boost::thread                  thread_;
    std::string                    exerciseMessage_;
    //@}
};

#endif // __ExerciseService_h_
