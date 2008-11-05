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

namespace tools
{
    class GeneralConfig;
}

namespace kernel
{
    class Controllers;
}

class ProgressPage;
class ExerciseList;
class DirectoryExerciseLister;

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
             ReplayPage( QWidgetStack* pages, Page_ABC& previous, kernel::Controllers& controllers, const tools::GeneralConfig& config );
    virtual ~ReplayPage();
    //@}

private slots:
    
    //! @name Operations
    //@{
    void OnStart();
    void OnSelectExercise( const QString& exercise, const QString& profile );
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
    void StartExercise( const QString& exercise );
    void ConfigureSession( const QString& exercise, const QString& session );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    kernel::Controllers&        controllers_;
    ProgressPage*               progressPage_;
    ExerciseList*               exercises_;
    QString                     exercise_;
    QListBox*                   sessionList_;
    std::auto_ptr< DirectoryExerciseLister > lister_;
    //@}
};

#endif // __ReplayPage_h_
