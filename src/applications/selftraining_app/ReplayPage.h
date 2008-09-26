// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayPage_h_
#define __ReplayPage_h_

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

class ExerciseList; 

// =============================================================================
/** @class  ReplayPage
    @brief  ReplayPage
*/
// Created: SBO 2008-02-21
// =============================================================================
class ReplayPage : public ContentPage
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus  );
    virtual ~ReplayPage();
    //@}

private slots:
    
    //! @name Operations
    //@{
    void OnStartExercise  ( const QString& exercise );
    void OnStart          ( );
    void OnSelectExercise ( const QString& exercise );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ReplayPage( const ReplayPage& );            //!< Copy constructor
    ReplayPage& operator=( const ReplayPage& ); //!< Assignment operator
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
    ExerciseList*       exercises_ ; 
    QListBox*           sessionList_;
    //@}
};

#endif // __ReplayPage_h_
