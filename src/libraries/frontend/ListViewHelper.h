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

#include <qlistview.h>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

class QListView;
class QListViewItem;

namespace frontend
{
    class CheckListItem : public QCheckListItem
    {
    public:
        CheckListItem( QCheckListItem *parent, const QString &text, bool recursive )
            : QCheckListItem( parent, text, QCheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        CheckListItem( QListViewItem *parent, const QString &text, bool recursive )
            : QCheckListItem( parent, text, QCheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        bool IsRecursive()
        {
            return recursive_;
        }

    private:
        bool recursive_;
    };

    QListViewItem* BuildExerciseData( const std::string& exercise, const tools::GeneralConfig& config, QListView* content, const tools::Loader_ABC& fileLoader );
    QListViewItem* BuildExerciseFeatures( const std::string& exercise, const tools::GeneralConfig& config, QListView* content );
}


#endif // __ListViewHelper_h_
