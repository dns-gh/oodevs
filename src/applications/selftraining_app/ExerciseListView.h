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

class Application;

// =============================================================================
/** @class  ExerciseListView
    @brief  Exercise list view
*/
// Created: LGY 2012-05-30
// =============================================================================
class ExerciseListView : public QTreeView
{
public:
    //! @name Constructors/Destructor
    //@{
             ExerciseListView( Application& app, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader );
    virtual ~ExerciseListView();
    //@}

    //! @name Operations
    //@{
    tools::Path GetExerciseName( const QModelIndex& index ) const;
    void AddExerciseEntry( const frontend::Exercise_ABC& exercise );
    QStandardItem* FindExerciseItem( const frontend::Exercise_ABC& exercise, QStandardItem* parent = 0 ) const;
    void DeleteExerciseEntry( const frontend::Exercise_ABC& exercise );
    const QStandardItem* GetSelectedExerciseItem() const;
    const frontend::Exercise_ABC* GetSelectedExercise() const;
    void Clear();
    bool Exists( const tools::Path& exercise ) const;
    //@}

private:
    //! @name Helpers
    //@{
    QStandardItem* FindExerciseItem( const QString& path, QStandardItem* parent = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    Application& app_;
    const tools::GeneralConfig& config_;
    const tools::Loader_ABC& fileLoader_;
    QStandardItemModel model_;
    QSortFilterProxyModel* proxy_;
    //@}
};

#endif // __ExerciseListView_h_
