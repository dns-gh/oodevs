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
//#include <QtGui/qmenu.h> // $$$$ ABR 2011-12-30: TODO: migrate menu to Qt4
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
        Category( const std::string name, bool separatorText = false ) : name_( name ), separatorText_( separatorText ) {}
        bool operator<( const Category& category ) const { return name_ < category.name_; }

        std::string name_;
        bool        separatorText_;
    };

    typedef boost::variant< QAction*, ContextMenu* >        T_MenuVariant;

    typedef std::map< int, T_MenuVariant >                  T_SubMenus;
    typedef T_SubMenus::const_iterator                    CIT_SubMenus;

    typedef std::map< Category, T_SubMenus >                T_Menus;
    typedef T_Menus::const_iterator                       CIT_Menus;

    typedef std::vector< std::string >                      T_BaseCategories;
    typedef T_BaseCategories::const_iterator              CIT_BaseCategories;
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
    void InitializeBaseCategories();
    void Clear();
    void Popup( const QPoint& where );
    void ConnectSubActions( const QObject* receiver, const char* member, int depth = 1 );
    //@}

    //! @name Operators
    //@{
    operator QWidget*() const;
    //@}

    //! @name 
    //@{
    T_MenuVariant InsertVariant( const std::string& category, T_MenuVariant& variant, bool separatorText = false, int index = -1 );
    QAction* InsertAction( const std::string& category, QAction* action, bool separatorText = false, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, bool separatorText = false, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, int index = -1 );
    QAction* InsertItem( const std::string& category, const QString& text, ContextMenu* popup, int index = -1 );
    ContextMenu* SubMenu( const std::string& category, const QString& text, bool textSeparator = false, int index = -1 );
    ContextMenu* SubMenu( const std::string& category, ContextMenu* popup, bool textSeparator = false, int index = -1 );
    //@}

private:
    //! @name Helpers
    //@{
    ContextMenu* FillMenu();
    void InternalFillMenu( const CIT_Menus& currentMenu );
    T_SubMenus& GetCategoryCreateIFN( const std::string& category, bool separatorText = false );
    //@}

private:
    //! @name Member data
    //@{
    T_BaseCategories baseCategories_;
    T_Menus          menus_;
    //@}
};

}

#endif // __ContextMenu_h_
