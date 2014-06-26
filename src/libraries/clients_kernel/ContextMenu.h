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

#pragma warning( push, 0 )
#include <Qt3Support/q3popupmenu.h>
//#include <QtGui/qmenu.h>
// $$$$ ABR 2011-12-30: TODO: migrate menu to Qt4, improve memory management, and simplify usage with QAction manipulation, and add QIcon parameter to Insert methodes
#pragma warning( pop )

#include <boost/variant.hpp>

class QAction;
class QKeySequence;
class QObject;
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
class ContextMenu : public Q3PopupMenu //QMenu
                  , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    struct Category
    {
        Category( const std::string& name, bool separatorText = false ) : name_( name ), separatorText_( separatorText ) {}
        bool operator<( const Category& category ) const { return name_ < category.name_; }

        std::string name_;
        bool        separatorText_;
    };

    typedef boost::variant< QAction*, ContextMenu* >        T_MenuVariant;

    typedef std::multimap< int, T_MenuVariant >             T_SubMenus;
    typedef T_SubMenus::const_iterator                    CIT_SubMenus;

    typedef std::map< Category, T_SubMenus >                T_Menus;
    typedef T_Menus::const_iterator                       CIT_Menus;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ContextMenu();
    explicit ContextMenu( QWidget* parent );
    virtual ~ContextMenu();
    //@}

    //! @name Operations
    //@{
    void Clear();
    void Popup( const QPoint& where );
    void ConnectSubActions( const QObject* receiver, const char* member, int depth = 1 );
    ContextMenu* FillMenu();
    T_SubMenus& GetCategoryCreateIFN( const std::string& category, bool separatorText = false );
    //@}

    //! @name Operators
    //@{
    operator QWidget*() const;
    //@}

    // Adds entries in the context menu. Entries ownership is transfered.
    // If index == -1, the entry is appended to the menu. Otherwise the entry
    // is inserted at "index", which is not unique: multiple entries can be
    // set at a given index, their relative position is undefined.
    T_MenuVariant InsertVariant( const std::string& category, T_MenuVariant& variant, bool separatorText = false, int index = -1 );
    QAction* InsertAction( const std::string& category, QAction* action, bool separatorText = false, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, bool separatorText = false, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, ContextMenu* popup, int index = -1 );
    ContextMenu* SubMenu( const std::string& category, const QString& text, bool textSeparator = false, int index = -1 );
    ContextMenu* SubMenu( const std::string& category, ContextMenu* popup, bool textSeparator = false, int index = -1 );

private:
    //! @name Helpers
    //@{
    void InternalFillMenu( const CIT_Menus& currentMenu );
    T_MenuVariant InsertMenuVariant( const std::string& category,
            T_MenuVariant variant, bool separatorText, int index );
    //@}

private:
    //! @name Member data
    //@{
    T_Menus          menus_;
    //@}
};

}

#endif // __ContextMenu_h_
