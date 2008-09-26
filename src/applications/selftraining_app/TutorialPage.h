// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TutorialPage_h_
#define __TutorialPage_h_

#include "ContentPage.h"

class QListBox;
class QTextBrowser;
class QTabWidget; 
class QLineEdit ; 

namespace gui 
{
    class LinkInterpreter_ABC ; 
}

namespace frontend
{
    class SpawnCommand ; 
    class InfoBubble ; 
}

namespace tools
{
    class GeneralConfig;
}

class Session ; 
class SessionRunningPage; 
class ExerciseList; 

// =============================================================================
/** @class  TutorialPage
    @brief  TutorialPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class TutorialPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TutorialPage( QWidgetStack* pages, Page_ABC& previous, SessionRunningPage& running, const tools::GeneralConfig& config, gui::LinkInterpreter_ABC& interpreter, boost::shared_ptr< Session > sessionStatus  );
    virtual ~TutorialPage();
    //@}

private slots:
    
    //! @name Operations
    //@{
    void OnStartExercise ( const QString& exercise );
    void OnStart         ( );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TutorialPage( const TutorialPage& );            //!< Copy constructor
    TutorialPage& operator=( const TutorialPage& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    void CreateSession( const QString& exercise, const QString& session );
    void StartSession( Session* session ); 
    //@}

private:
    
    boost::shared_ptr< Session > sessionStatus_ ; 

    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    ExerciseList*               exercises_ ; 
    gui::LinkInterpreter_ABC&   interpreter_ ; 
    QLabel*                     statusLabel_ ; 
    SessionRunningPage&         running_ ; 
    //@}
};

#endif // __TutorialPage_h_
