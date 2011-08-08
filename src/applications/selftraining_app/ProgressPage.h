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
             ProgressPage( Q3WidgetStack* pages, Page_ABC& previous, const QString& title );
    virtual ~ProgressPage();
    //@}

    //! @name Operations
    //@{
    void Attach( boost::shared_ptr< frontend::Process_ABC > process );
    virtual void NotifyStopped();
    virtual void NotifyError( const std::string& error );
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

private:
    //! @name Member data
    //@{
    boost::weak_ptr< frontend::Process_ABC > process_;
    QLabel* label_;
    Q3ProgressBar* progressBar_;
    QTimer* timer_;
    //@}
};

#endif // __ProgressPage_h_
