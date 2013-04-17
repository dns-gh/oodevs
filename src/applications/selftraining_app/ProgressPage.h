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
#include "frontend/ProcessObserver_ABC.h"
#include <boost/weak_ptr.hpp>

namespace frontend
{
    class Process_ABC;
}

class Application;

// =============================================================================
/** @class  ProgressPage
    @brief  ProgressPage
*/
// Created: SBO 2008-10-14
// =============================================================================
class ProgressPage : public ContentPage
                   , public frontend::ProcessObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProgressPage( Application& app, QStackedWidget* pages, Page_ABC& previous );
    virtual ~ProgressPage();
    //@}

    //! @name Operations
    //@{
    void Attach( boost::shared_ptr< frontend::Process_ABC > process );
    virtual void NotifyStopped();
    virtual void NotifyError( const std::string& error, std::string /*commanderEndpoint*/ = "" );
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateProgress();
    void OnNotifyStopped();
    void OnNotifyError( const QString& message );
    //@}

private:
    //! @name Member data
    //@{
    Application& app_;
    boost::shared_ptr< frontend::Process_ABC > process_;
    QLabel* label_;
    QProgressBar* progressBar_;
    QTimer* timer_;
    //@}
};

#endif // __ProgressPage_h_
