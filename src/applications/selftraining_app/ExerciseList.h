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
             ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const ExerciseLister_ABC& lister, const std::string& subDir = "", bool showBrief = true, bool showProfile =true );
    virtual ~ExerciseList();
    //@}

    //! @name Accessors
    //@{
    const QString GetHighlight() const;
    //@}

signals:
    //! @name Signals
    //@{
    void Select( const QString& exercise, const QString& profile );
    //@}

public slots:
    //! @name Slots
    //@{
    void Update();
    void SelectExercise( int index );
    void SelectProfile( const Profile& profile );
    void Clear();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExerciseList( const ExerciseList& );            //!< Copy constructor
    ExerciseList& operator=( const ExerciseList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    QString BuildExercisePath() const;
    void ReadBriefingText( xml::xistream& xis );
    QString GetExerciseDisplayName( const QString& exercise ) const ; 
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig&  config_;
    const std::string            subDir_;
    QListBox*                    exercises_;
    QLabel*                      briefingImage_;
    QTextEdit*                   briefingText_;
    ProfileList*                 profiles_;
    bool                         showBrief_;
    QStringList                  exercisesList_;
    const ExerciseLister_ABC&    lister_;
 //@}
};

#endif // __ExerciseList_h_
