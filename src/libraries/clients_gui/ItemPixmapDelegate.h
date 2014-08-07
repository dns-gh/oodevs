// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ItemPixmapDelegate_h_
#define __ItemPixmapDelegate_h_


namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
class StandardModel;

// =============================================================================
/** @class  ItemPixmapDelegate
    @brief  ItemPixmapDelegate
*/
// Created: JSR 2012-09-14
// =============================================================================
class ItemPixmapDelegate : public QItemDelegate

{
    typedef std::function< std::vector< const QPixmap* >( const kernel::Entity_ABC& ) > T_PixmapGetter;

public:
    //! @name Constructors/Destructor
    //@{
             ItemPixmapDelegate( const StandardModel& dataModel, T_PixmapGetter getter, QObject *parent, int height = 0 );
    virtual ~ItemPixmapDelegate();
    //@}

    //! @name Operations
    //@{
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void DrawPixmap( QPainter* painter, const QStyleOptionViewItem& option, const std::vector< const QPixmap* >& pixmaps, const QModelIndex& index ) const;
    //@}

private:
    //! @name Member data
    //@{
    const StandardModel& dataModel_;
    T_PixmapGetter getter_;
    int height_;
    //@}
};

}

#endif // __ItemPixmapDelegate_h_
