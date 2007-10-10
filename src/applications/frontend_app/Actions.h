// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Actions_h_
#define __Actions_h_

#include <qaction.h>

// =============================================================================
/** @class  Actions
    @brief  Actions
*/
// Created: AGE 2007-10-10
// =============================================================================
class Actions : public QObject
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Actions( QObject* parent );
    virtual ~Actions();
    //@}

    //! @name Preparation
    //@{
    QAction& CreateTerrain();
    QAction& CreateExercise();
    QAction& EditExercise();
    //@}

    //! @name Gaming
    //@{
    QAction& StartExercise();
    QAction& RestartExercise();
    QAction& JoinExercise();
    //@}

    //! @name Analysis
    //@{
    QAction& StartAnalysis();
    QAction& JoinAnalysis();
    //@}

private:
    //! @name Helpers
    //@{
    QAction& CreateAction( const QString& name, const char* cname ); 
    //@}
};

#endif // __Actions_h_
