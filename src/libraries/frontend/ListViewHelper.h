// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ListViewHelper_h_
#define __ListViewHelper_h_

#include <tools/Path.h>
#include <vector>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

namespace frontend
{
    class CheckListItem : public QStandardItem
    {
    public:
        CheckListItem( const tools::Path& path, bool recursive )
            : QStandardItem( path.Normalize().ToUTF8().c_str() )
            , recursive_( recursive )
        {
            setCheckable( true );
            if( !recursive )
            {
                setEnabled( false );
                setCheckState( Qt::Checked );
            }
        }

        bool IsRecursive()
        {
            return recursive_;
        }

    private:
        bool recursive_;
    };

    void BuildExerciseData( const tools::Path& exercise,
        const tools::GeneralConfig& config, QStandardItemModel& content, const tools::Loader_ABC& loader );
    void RemoveExerciseData( const tools::Path& exercise, const std::vector< tools::Path >& exercises,
        const tools::GeneralConfig& config, QStandardItemModel& model, const tools::Loader_ABC& loader );

    void BuildExerciseFeatures( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& model );
    void RemoveExerciseFeatures( const tools::Path& exercise, QStandardItemModel& model );
}

#endif // __ListViewHelper_h_
