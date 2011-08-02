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

#include <Qt3Support/q3action.h>

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
    Q3Action& CreateTerrain();
    Q3Action& CreateExercise();
    Q3Action& EditExercise();
    //@}

    //! @name Gaming
    //@{
    Q3Action& StartExercise();
    Q3Action& RestartExercise();
    Q3Action& JoinExercise();
    //@}

    //! @name Analysis
    //@{
    Q3Action& StartAnalysis();
    Q3Action& JoinAnalysis();
    //@}

    //! @name Administration
    //@{
    Q3Action& InstallPackage();
    Q3Action& CreatePackage();
    //@}

private:
    //! @name Helpers
    //@{
    Q3Action& CreateAction( const QString& name, const char* cname );
    //@}

private:
    //! @name Member data
    //@{
    Q3ActionGroup* group_;
    //@}
};

#endif // __Actions_h_
