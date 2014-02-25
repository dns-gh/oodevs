// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseList_h_
#define __ExerciseList_h_

#include "clients_gui/WidgetLanguageObserver_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/ElementObserver_ABC.h>

namespace frontend
{
    class Exercise_ABC;
    class Profile;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

class Application;
class ExerciseProperties;
class ProfileList;
class ExerciseListView;

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: RDS 2008-08-27
// =============================================================================
class ExerciseList : public gui::WidgetLanguageObserver_ABC< QWidget >
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
                   , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseList( Application& app, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, bool showBrief = true, bool showProfile = true, bool showParams = true, bool enableParams = true );
    virtual ~ExerciseList();
    //@}

    //! @name Operations
    //@{
    bool Exists( const tools::Path& exercise ) const;
    bool IsPropertiesValid() const;
    bool ChangeExerciceParameters();
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    void ExercisePropertiesChanged();
    //@}

public slots:
    //! @name Slots
    //@{
    void Clear();
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectExercise( const QItemSelection&, const QItemSelection& );
    void SelectProfile( const frontend::Profile& profile );
    void OnExercisePropertiesChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyUpdated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyDeleted( const frontend::Exercise_ABC& exercise );
    virtual void OnLanguageChanged();
    const frontend::Exercise_ABC* GetSelectedExercise() const;
    void UpdateExerciseEntry( const frontend::Exercise_ABC& exercise );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ExerciseListView* exercises_;
    ProfileList* profiles_;
    ExerciseProperties* properties_;
    QLabel* exerciseLabel_;
    QLabel* profileLabel_;
 //@}
};

#endif // __ExerciseList_h_
