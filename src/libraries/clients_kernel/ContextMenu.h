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

class QPopupMenu;
class QObject;
class QWidget;
class QPoint;
class QString;
class QKeySequence;

namespace kernel
{

// =============================================================================
/** @class  ContextMenu
    @brief  Context menu wrapper
*/
// Created: AGE 2006-08-04
// =============================================================================
class ContextMenu
{

public:
    //! @name Constructors/Destructor
    //@{
             ContextMenu();
    virtual ~ContextMenu();
    //@}

    //! @name Operations
    //@{
    void AddCategory( const std::string& text, int position = -1 );

    void Clear();
    void SetItemEnabled( int id, bool enable );
    void SetItemParameter( int id, int parameter );

    int InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member );
    int InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel, int id = -1 );
    int InsertItem( const std::string& category, const QString& text, int id = -1 );
    int InsertItem( const std::string& category, const QString& text, QPopupMenu* popup, int id = -1 );
    int InsertItem( const std::string& category, QWidget* widget, int id = -1 );
    
    QPopupMenu* SubMenu( const std::string& category, const QString& text );

    void Popup( const QPoint& where );
    //@}

    //! @name Operators
    //@{
    operator QWidget*() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ContextMenu( const ContextMenu& );            //!< Copy constructor
    ContextMenu& operator=( const ContextMenu& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string >     T_Categories;
    typedef T_Categories::iterator        IT_Categories;
    typedef T_Categories::const_iterator CIT_Categories;
    //@}

    //! @name Helpers
    //@{
    int InsertCategory( const std::string& category );
    void InsertSeparators();
    //@}

private:
    //! @name Member data
    //@{
    QPopupMenu* menu_;
    T_Categories categories_;
    T_Categories insertedCategories_;
    //@}
};

}

#endif // __ContextMenu_h_
