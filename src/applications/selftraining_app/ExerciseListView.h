// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseListView_h_
#define __ExerciseListView_h_

namespace frontend
{
    class Exercise_ABC;
}

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

// =============================================================================
/** @class  ExerciseListView
    @brief  Exercise list view
*/
// Created: LGY 2012-05-30
// =============================================================================
class ExerciseListView : public Q3ListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ExerciseListView( QWidget* widget, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
    virtual ~ExerciseListView();
    //@}

    //! @name Operations
    //@{
    QString GetItemName( Q3ListViewItem* item ) const;
    void AddExerciseEntry( const frontend::Exercise_ABC& exercise );
    void DeleteExerciseEntry( const frontend::Exercise_ABC& exercise );
    //@}

private:
    //! @name Helpers
    //@{
    Q3ListViewItem* FindExerciseItem( const QString& path ) const;
    QString GetExerciseDisplayName( const QString& exercise ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    //@}
};

#endif // __ExerciseListView_h_
