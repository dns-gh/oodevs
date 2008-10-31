// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ProgressPage_h_
#define __ProgressPage_h_

#include "ContentPage.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class Process_ABC;
}

class QProgressBar;
class QTimer;

// =============================================================================
/** @class  ProgressPage
    @brief  ProgressPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class ProgressPage : public ContentPage
                   , public kernel::Observer_ABC
                   , public kernel::ElementObserver_ABC< boost::shared_ptr< frontend::Process_ABC > >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProgressPage( QWidgetStack* pages, Page_ABC& previous, const QString& title, kernel::Controllers& controllers );
    virtual ~ProgressPage();
    //@}

    //! @name Operations
    //@{
    void Attach( boost::shared_ptr< frontend::Process_ABC > process );
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateProgress();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProgressPage( const ProgressPage& );            //!< Copy constructor
    ProgressPage& operator=( const ProgressPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const boost::shared_ptr< frontend::Process_ABC >& process );
    virtual void NotifyDeleted( const boost::shared_ptr< frontend::Process_ABC >& process );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    boost::shared_ptr< frontend::Process_ABC > process_;
    QLabel* label_;
    QProgressBar* progressBar_;
    QTimer* timer_;
    //@}
};

#endif // __ProgressPage_h_
