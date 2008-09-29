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

#include "clients_kernel/ElementObserver_ABC.h" 
#include "clients_kernel/Observer_ABC.h" 
#include "ContentPage.h" 

namespace tools
{
    class GeneralConfig ; 
}

namespace kernel 
{
    class Controllers; 
}

class Session; 
class SessionStatus ; 
class QProgressBar; 
class QTimer ; 

// =============================================================================
/** @class  SessionRunningPage
    @brief  SessionRunningPage
*/
// Created: RDS 2008-09-08
// =============================================================================
class SessionRunningPage : public ContentPage
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< SessionStatus > 
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SessionRunningPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, boost::shared_ptr< Session > sessionStatus);
    virtual ~SessionRunningPage();
    //@}

    //! @name Operations
    //@{
    void SetSession( Session* session ) ; 
    void NotifyUpdated( const SessionStatus& status ) ; 
    //@}

public slots:

    void UpdateProgress(); 

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
    boost::shared_ptr< Session > sessionStatus_ ; 
    const tools::GeneralConfig& config_ ; 
    QLabel* status_ ; 
    QTimer* timer_ ; 
    QProgressBar* progressBar_ ;
    kernel::Controllers& controllers_ ; 
    //@}
};

#endif // __SessionRunningPage_h_
