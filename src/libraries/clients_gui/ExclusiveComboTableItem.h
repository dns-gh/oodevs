// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExclusiveComboTableItem_h_
#define __ExclusiveComboTableItem_h_

#include <qtable.h>

namespace gui
{

// =============================================================================
/** @class  ExclusiveComboTableItem
    @brief  ComboTableItem allowing each item to be selected once in table column
*/
// Created: SBO 2006-06-27
// =============================================================================
class ExclusiveComboTableItem : public QComboTableItem
{
public:
    //! @name Constructors/Destructor
    //@{
    ExclusiveComboTableItem( QTable* table, const QStringList& list )
        : QComboTableItem( table, list )
        , list_( list ) {}

    ~ExclusiveComboTableItem() {}
    //@}

    //! @name Operations
    //@{
    QWidget* createEditor() const
    {
        QStringList list( list_ );
        for( int r = 0; r < table()->numRows(); ++r )
        {
            const QString text = table()->text( r, col() );
            if( r != row() && !text.isEmpty() )
                list.remove( text );
        }
        const_cast< ExclusiveComboTableItem* >( this )->setStringList( list );
        return QComboTableItem::createEditor();
    }
    //@}

private:
    //! @name Member data
    //@{
    QStringList list_;
    //@}
};

}

#endif // __ExclusiveComboTableItem_h_
