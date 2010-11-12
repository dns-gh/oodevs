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
    class RemoteExercise;
    class ExerciseFilter_ABC;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class GeneralConfig;
}

namespace xml
{
    class xistream;
}

class ProfileList;
class Profile;
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
             ExerciseList( QWidget* parent, const tools::GeneralConfig& config, kernel::Controllers& controllers, const std::string& subDir = "", bool showBrief = true, bool showProfile = true, bool showParams = true, bool enableParams = true );
    virtual ~ExerciseList();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const frontend::ExerciseFilter_ABC& filter );
    bool Exists( const QString& exercise ) const;
    void ChangeExerciceParameters( const std::string& exercice );
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const frontend::Exercise_ABC& exercise, const Profile& profile );
    void ClearSelection();
    //@}

public slots:
    //! @name Slots
    //@{
    void Update();
    void Clear();
    void ComboChanged( int index );
    //@}

private slots:
    //! @name Slots
    //@{
    void SelectExercise( QListViewItem* item );
    void SelectProfile( const Profile& profile );
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
    void ReadBriefingText( xml::xistream& xis );
    QString GetExerciseDisplayName( const QString& exercise ) const;
    void AddExerciseEntry( const frontend::Exercise_ABC& exercise );
    void UpdateExerciseEntry( const frontend::Exercise_ABC& exercise );
    void DeleteExerciseEntry( const frontend::Exercise_ABC& exercise );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&  config_;
    kernel::Controllers&         controllers_;
    const std::string            subDir_;
    QListView*                   exercises_;
    QLabel*                      briefingImage_;
    QTextEdit*                   briefingText_;
    ProfileList*                 profiles_;
    bool                         showBrief_;
    const QString                language_;
    bool                         parametersChanged_;    
    QComboBox*                   editTerrainList_;
    QComboBox*                   editModelList_;
    const frontend::ExerciseFilter_ABC* filter_;
    std::auto_ptr< frontend::ExerciseFilter_ABC > defaultFilter_;
 //@}
};

#endif // __ExerciseList_h_
