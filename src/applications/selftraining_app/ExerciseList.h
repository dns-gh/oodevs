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
class QListView;
class QListViewItem;

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: RDS 2008-08-27
// =============================================================================
class ExerciseList : public QVBox
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< frontend::Exercise_ABC >
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
    void ChangeExerciceParameters();
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const frontend::Exercise_ABC& exercise, const frontend::Profile& profile );
    void ClearSelection();
    //@}

public slots:
    //! @name Slots
    //@{
    void Clear();
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectExercise( QListViewItem* item );
    void SelectProfile( const frontend::Profile& profile );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseList( const ExerciseList& );            //!< Copy constructor
    ExerciseList& operator=( const ExerciseList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyUpdated( const frontend::Exercise_ABC& exercise );
    virtual void NotifyDeleted( const frontend::Exercise_ABC& exercise );

    virtual void customEvent( QCustomEvent* e );
    const frontend::Exercise_ABC* GetSelectedExercise() const;
    QString GetExerciseDisplayName( const QString& exercise ) const;
    void AddExerciseEntry( const frontend::Exercise_ABC& exercise );
    void UpdateExerciseEntry( const frontend::Exercise_ABC& exercise );
    void DeleteExerciseEntry( const frontend::Exercise_ABC& exercise );
    QListViewItem* FindExerciseItem( const QString& path ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    kernel::Controllers& controllers_;
    QListView* exercises_;
    ProfileList* profiles_;
    const frontend::ExerciseFilter_ABC* filter_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > defaultFilter_;
    ExerciseProperties* properties_;
 //@}
};

#endif // __ExerciseList_h_
