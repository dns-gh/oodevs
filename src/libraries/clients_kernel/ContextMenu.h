// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ContextMenu_h_
#define __ContextMenu_h_

#include <boost/noncopyable.hpp>

class Q3PopupMenu;
class QAction;
class QKeySequence;
class QObject;
class QPixmap;
class QPoint;
class QString;
class QWidget;

namespace kernel
{

// =============================================================================
/** @class  ContextMenu
    @brief  Context menu wrapper
*/
// Created: AGE 2006-08-04
// =============================================================================
class ContextMenu : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextMenu();
    virtual ~ContextMenu();
    //@}

    //! @name Operations
    //@{
    void AddCategory( const std::string& text );

    void Clear();
    void SetItemEnabled( int id, bool enable );
    void SetItemParameter( int id, int parameter );
    void SetPixmap( int id, const QPixmap& pixmap );

    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member );
    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel, int id = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, int id = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, Q3PopupMenu* popup, int id = -1 );

    Q3PopupMenu* SubMenu( const std::string& category, const QString& text );

    void Popup( const QPoint& where );
    //@}

    //! @name Operators
    //@{
    operator QWidget*() const;
    //@}

private:
    //! @name Types
    //@{
    struct CategoryTextKey
    {
        CategoryTextKey( const std::string category, const std::string text ) : category_( category ), text_( text ) {}
        bool operator<( const CategoryTextKey& categoryText ) const { return ( category_ == categoryText.category_ ) ? ( text_ < categoryText.text_ ) : ( category_ < categoryText.category_ ); }

        std::string category_;
        std::string text_;
    };

    typedef std::vector< std::string > T_OrderedCategories;
    typedef std::map< CategoryTextKey, Q3PopupMenu* > T_CategoryMenus;
    typedef std::map< std::string, QAction* > T_Categories;
    //@}

    //! @name Helpers
    //@{
    QAction* InsertCategory( const std::string& category );
    //@}

private:
    //! @name Member data
    //@{
    Q3PopupMenu* menu_;
    T_OrderedCategories categories_;
    T_Categories separators_;
    T_CategoryMenus menus_;
    //@}
};

}

#endif // __ContextMenu_h_
