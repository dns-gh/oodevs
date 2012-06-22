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

#pragma warning( push, 0 )
#include <Qt3Support/q3table.h>
#pragma warning( pop )

#include "ComboTableItem.h"
#include <QtGui/qcombobox.h>

namespace gui
{

// =============================================================================
/** @class  ExclusiveComboTableItem
    @brief  ComboTableItem allowing each item to be selected once in table column
*/
// Created: SBO 2006-06-27
// =============================================================================
class ExclusiveComboTableItem : public ComboTableItem
{
public:
    //! @name Constructors/Destructor
    //@{
    ExclusiveComboTableItem( Q3Table* table, const QStringList& list )
        : ComboTableItem( table, list, Q3TableItem::WhenCurrent )
        , list_( list ) {}

    virtual ~ExclusiveComboTableItem() {}
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

        QString current = text();
        const_cast< ExclusiveComboTableItem* >( this )->SetTexts( list );
        if( list.contains( current ) )
            const_cast< ExclusiveComboTableItem* >( this )->SetCurrentText( current );

        QWidget* createdWidget = ComboTableItem::createEditor();
        if( createdWidget )
        {
            QComboBox* createdCombo = dynamic_cast< QComboBox* >( createdWidget );
            if( createdCombo )
                createdCombo->setCurrentText( current );
        }
        return createdWidget;
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
