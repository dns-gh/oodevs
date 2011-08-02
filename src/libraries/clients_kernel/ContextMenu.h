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
class QObject;
class QWidget;
class QPoint;
class QString;
class QKeySequence;
class QPixmap;

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
    void SetChecked( int id, bool checked );
    void SetPixmap( int id, const QPixmap& pixmap );

    int InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member );
    int InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel, int id = -1 );
    int InsertItem( const std::string& category, const QString& text, int id = -1 );
    int InsertItem( const std::string& category, const QString& text, Q3PopupMenu* popup, int id = -1 );

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
    typedef std::vector< std::string > T_OrderedCategories;
    typedef std::map< std::string, Q3PopupMenu* > T_CategoryMenus;
    //@}

    //! @name Helpers
    //@{
    int InsertCategory( const std::string& category );
    //@}

private:
    //! @name Member data
    //@{
    Q3PopupMenu* menu_;
    T_OrderedCategories categories_;
    T_CategoryMenus menus_;
    //@}
};

}

#endif // __ContextMenu_h_
