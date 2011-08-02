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

#include <Qt3Support/q3listview.h>

namespace tools
{
    class GeneralConfig;
    class Loader_ABC;
}

class Q3ListView;
class Q3ListViewItem;

namespace frontend
{
    class CheckListItem : public Q3CheckListItem
    {
    public:
        CheckListItem( Q3CheckListItem *parent, const QString &text, bool recursive )
            : Q3CheckListItem( parent, text, Q3CheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        CheckListItem( Q3ListViewItem *parent, const QString &text, bool recursive )
            : Q3CheckListItem( parent, text, Q3CheckListItem::CheckBox )
            , recursive_( recursive )
        {}

        bool IsRecursive()
        {
            return recursive_;
        }

    private:
        bool recursive_;
    };

    Q3ListViewItem* BuildExerciseData( const std::string& exercise, const tools::GeneralConfig& config, Q3ListView* content, const tools::Loader_ABC& fileLoader );
    Q3ListViewItem* BuildExerciseFeatures( const std::string& exercise, const tools::GeneralConfig& config, Q3ListView* content );
}

#endif // __ListViewHelper_h_
