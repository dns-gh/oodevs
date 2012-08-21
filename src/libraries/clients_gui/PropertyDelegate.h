// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertyDelegate_h
#define gui_PropertyDelegate_h

namespace kernel
{
    class EditorFactory_ABC;
}

namespace gui
{
// =============================================================================
/** @class  PropertyDelegate
    @brief  Property delegate
*/
// Created: LGY 2012-08-13
// =============================================================================
class PropertyDelegate : public QItemDelegate
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertyDelegate( kernel::EditorFactory_ABC& factory );
    virtual ~PropertyDelegate();
    //@}

    //! @name Operations
    //@{
    QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::EditorFactory_ABC& factory_;
    //@}
};

}

#endif // gui_PropertyDelegate_h
