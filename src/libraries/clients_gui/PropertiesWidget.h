// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesWidget_h_
#define __PropertiesWidget_h_

#include "clients_kernel/Displayer_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
    class EditorFactory_ABC;
    class DictionaryUpdated;
}

namespace gui
{
    class PropertiesTable;
    class TableItemDisplayer;

// =============================================================================
/** @class  PropertiesWidget
    @brief  PropertiesWidget
*/
// Created: AGE 2006-10-18
// =============================================================================
class PropertiesWidget : public QWidget
                       , public kernel::Displayer_ABC
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::DictionaryUpdated >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesWidget( kernel::Controller& controller, QWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer );
    virtual ~PropertiesWidget();
    //@}

    //! @name Operations
    //@{
    void Clear();
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesWidget( kernel::Controller& controller, PropertiesWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer );
    PropertiesWidget( const PropertiesWidget& );            //!< Copy constructor
    PropertiesWidget& operator=( const PropertiesWidget& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< PropertiesWidget* >  T_SubWidgets;
    typedef std::map< QString, std::size_t > T_SubCategories;
    typedef T_SubCategories::const_iterator CIT_SubCategories;
    //@}

    //! @name Helpers
    //@{
    void FillUp( const QString& name, bool root = false );
    kernel::Displayer_ABC& SubItem( const QString& name, const QString& path );
    PropertiesWidget* CreateWidget( const QString& subItem );

    void UpdatePath( const kernel::DictionaryUpdated& message, const QString& name, PropertiesWidget& parent );
    void ClearPath( const QString& name );

    virtual kernel::Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    virtual void NotifyUpdated( const kernel::DictionaryUpdated& message );
    virtual void NotifyDeleted( const kernel::DictionaryUpdated& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::EditorFactory_ABC& factory_;
    QGridLayout* layout_;
    QToolButton* button_;
    T_SubCategories categories_;
    T_SubWidgets subWidgets_;
    PropertiesTable* table_;
    TableItemDisplayer& displayer_;
    Q3VBox* globalLayout;
    //@}
};

}

#endif // __PropertiesWidget_h_
