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

#include <boost/noncopyable.hpp>
#include "clients_gui/LanguageChangeObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace frontend
{
    class Exercise_ABC;
    class ExerciseFilter_ABC;
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

class ExerciseProperties;
class ProfileList;
class ExerciseListView;

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: RDS 2008-08-27
// =============================================================================
class ExerciseList : public gui::LanguageChangeObserver_ABC< Q3VBox >
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
                   , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers, bool showBrief = true, bool showProfile = true, bool showParams = true, bool enableParams = true );
    virtual ~ExerciseList();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const frontend::ExerciseFilter_ABC& filter );
    bool Exists( const QString& exercise ) const;
    bool IsPropertiesValid() const;
    void ChangeExerciceParameters();
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
    void SelectExercise( Q3ListViewItem* item );
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
    virtual void customEvent( QCustomEvent* e );
    const frontend::Exercise_ABC* GetSelectedExercise() const;
    void UpdateExerciseEntry( const frontend::Exercise_ABC& exercise );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ExerciseListView* exercises_;
    ProfileList* profiles_;
    const frontend::ExerciseFilter_ABC* filter_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > defaultFilter_;
    ExerciseProperties* properties_;
    QLabel* exerciseLabel_;
    QLabel* profileLabel_;
 //@}
};

#endif // __ExerciseList_h_
