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
        CheckListItem( const QString text, bool recursive )
            : QStandardItem( text )
            , recursive_( recursive )
        {
            setCheckable( true );
        }

        bool IsRecursive()
        {
            return recursive_;
        }

    private:
        bool recursive_;
    };

    void BuildExerciseData( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& content, const tools::Loader_ABC& fileLoader );
    void BuildExerciseFeatures( const tools::Path& exercise, const tools::GeneralConfig& config, QStandardItemModel& content );
}

#endif // __ListViewHelper_h_
