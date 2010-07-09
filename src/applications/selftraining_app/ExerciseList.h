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
class ExerciseLister_ABC;
class QListView;
class QListViewItem;

// =============================================================================
/** @class  ExerciseList
    @brief  ExerciseList
*/
// Created: RDS 2008-08-27
// =============================================================================
class ExerciseList : public QVBox
{

    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const ExerciseLister_ABC& lister, const std::string& subDir = "", bool showBrief = true, bool showProfile = true );
    virtual ~ExerciseList();
    //@}

    //! @name Operations
    //@{
    bool Exists( const QString& exercise ) const;
    void ChangeExerciceParameters( const std::string& exercice );

    //@}

signals:
    //! @name Signals
    //@{
    void Select( const QString& exercise, const Profile& profile );
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
    virtual void customEvent( QCustomEvent* e );
    QString BuildExercisePath() const;
    void ReadBriefingText( xml::xistream& xis );
    QString GetExerciseDisplayName( const QString& exercise ) const;
    void AddExerciseEntry( const QString& exercise );
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&  config_;
    const std::string            subDir_;
    QListView*                   exercises_;
    QLabel*                      briefingImage_;
    QTextEdit*                   briefingText_;
    ProfileList*                 profiles_;
    bool                         showBrief_;
    const ExerciseLister_ABC&    lister_;
    const QString                language_;
    bool                         parametersChanged_;
    
    QComboBox* editTerrainList_;
    QComboBox* editModelList_;
 //@}
};

#endif // __ExerciseList_h_
