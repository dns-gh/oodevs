// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __SessionRunningPage_h_
#define __SessionRunningPage_h_

#include "ContentPage.h" 

namespace tools
{
    class GeneralConfig ; 
}

class SessionStatus; 

class QProgressBar; 
class QTimer ; 

// =============================================================================
/** @class  SessionRunningPage
    @brief  SessionRunningPage
*/
// Created: RDS 2008-09-08
// =============================================================================
class SessionRunningPage : public ContentPage
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SessionRunningPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, boost::shared_ptr< SessionStatus > sessionStatus);
    virtual ~SessionRunningPage();
    //@}

    //! @name Operations
    //@{
    void SetSession( SessionStatus* session ) ; 
    //@}

public slots:

    void UpdateProgress(); 
    void OnSessionPhase ( const QString& ); 

private:
    //! @name Copy/Assignment
    //@{
    SessionRunningPage( const SessionRunningPage& );            //!< Copy constructor
    SessionRunningPage& operator=( const SessionRunningPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void StartProgressListening(); 
    void StopProgressListening(); 
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< SessionStatus > sessionStatus_ ; 
    const tools::GeneralConfig& config_ ; 
    QLabel* status_ ; 
    QTimer* timer_ ; 
    QProgressBar* progressBar_ ; 
    //@}
};

#endif // __SessionRunningPage_h_
