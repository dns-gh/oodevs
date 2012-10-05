// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_GridDelegate_h__
#define __ADN_GridDelegate_h__

// =============================================================================
/** @class  ADN_GridDelegate
    @brief  ADN_GridDelegate
*/
// Created: JSR 2012-10-03
// =============================================================================
class ADN_GridDelegate : public QItemDelegate
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_GridDelegate( QObject* parent ) : QItemDelegate( parent ) {}
    virtual ~ADN_GridDelegate() {}
    //@}

    //! @name From QItemDelegate
    //@{
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
    {
        QItemDelegate::paint( painter, option, index );
        if( index.isValid() )
        {
            painter->save();
            painter->setPen( Qt::SolidLine );
            painter->drawLine( option.rect.topLeft(), option.rect.topRight() ); // top
            painter->drawLine( option.rect.topRight(), option.rect.bottomRight() ); // right
            painter->restore();
        }
    }
    //@}
};

#endif // __ADN_GridDelegate_h__
