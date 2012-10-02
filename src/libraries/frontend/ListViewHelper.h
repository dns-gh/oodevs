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

#pragma warning( push, 0 )
#include <Qt3Support/q3listview.h>
#pragma warning( pop )

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

class Q3ListView;
class Q3ListViewItem;

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

    void BuildExerciseData( const std::string& exercise, const tools::GeneralConfig& config, QStandardItemModel& content, const tools::Loader_ABC& fileLoader );
    void BuildExerciseFeatures( const std::string& exercise, const tools::GeneralConfig& config, QStandardItemModel& content );
}

#endif // __ListViewHelper_h_
