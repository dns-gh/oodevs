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

namespace frontend
{
    class SpawnCommand ; 
}

namespace boost 
{
    class thread ; 
}

// =============================================================================
/** @class  SessionStatus
    @brief  SessionStatus
*/
// Created: RDS 2008-08-21
// =============================================================================
class SessionStatus : public QObject 
{

    Q_OBJECT ; 

public:
    //! @name Constructors/Destructor
    //@{
             SessionStatus( frontend::SpawnCommand* simulation=NULL, frontend::SpawnCommand* gui=NULL );
    virtual ~SessionStatus();
    //@}

    //! @name Operations
    //@{
    void Start(); 
    bool IsSimRunning() ; 
    bool IsGUIRunning() ; 
    bool HasRunningProcess() ; 
    void StopSimulation( bool confirm = false ); 
    void StopGUI( bool confirm = false ); 
    unsigned int GetPercentage(); 
    //@}

signals:

    //! @name signals
    //@{
    void SimulationStarted(); 
    void GUIStarted(); 
    void GUIClosed(); 
    void Feedback ( const QString& ) ; 
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SessionStatus( const SessionStatus& );            //!< Copy constructor
    SessionStatus& operator=( const SessionStatus& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Stop ( std::auto_ptr< frontend::SpawnCommand >& command, bool confirm = false ) ; 
    //@}

private:

    //! @name Member data
    //@{
    std::auto_ptr< frontend::SpawnCommand > simulation_ ; 
    std::auto_ptr< frontend::SpawnCommand > gui_ ; 
    std::auto_ptr< boost::thread > thread_ ; 
    //@}

    void ThreadStart(); 

};

#endif // __SessionStatus_h_
