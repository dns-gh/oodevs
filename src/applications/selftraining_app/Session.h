// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Session_h_
#define __Session_h_

namespace frontend
{
    class SpawnCommand;
}

namespace boost
{
    class thread;
}

namespace kernel
{
    class Controller;
}

// =============================================================================
/** @class  Session
    @brief  Session
*/
// Created: RDS 2008-08-21
// =============================================================================
class Session : public QObject 
{
public:
    //! @name Constructors/Destructor
    //@{
             Session( kernel::Controller& controller, frontend::SpawnCommand* simulation = 0, frontend::SpawnCommand* gui = 0 );
    virtual ~Session();
    //@}

    //! @name Operations
    //@{
    void Start();
    bool IsSimRunning() const;
    bool IsGUIRunning() const;
    bool HasRunningProcess();
    void StopSimulation( bool confirm = false );
    void StopGUI( bool confirm = false );
    unsigned int GetPercentage() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Session( const Session& );            //!< Copy constructor
    Session& operator=( const Session& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Stop( std::auto_ptr< frontend::SpawnCommand >& command, bool confirm = false );
    void ThreadStart();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< frontend::SpawnCommand > simulation_;
    std::auto_ptr< frontend::SpawnCommand > gui_;
    std::auto_ptr< boost::thread > thread_;
    kernel::Controller& controller_;
    //@}
};

#endif // __Session_h_
