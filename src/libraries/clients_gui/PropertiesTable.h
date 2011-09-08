// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesTable_h_
#define __PropertiesTable_h_

#include "clients_kernel/Displayer_ABC.h"

namespace kernel
{
    class EditorFactory_ABC;
    class Property_ABC;
}

namespace gui
{
    class TableItemDisplayer;

// =============================================================================
/** @class  PropertiesTable
    @brief  PropertiesTable
*/
// Created: SBO 2006-10-17
// =============================================================================
class PropertiesTable : public Q3Table
                      , public kernel::Displayer_ABC
                      , public tools::Caller< kernel::Property_ABC* >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesTable( QWidget* parent, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer );
    virtual ~PropertiesTable();
    //@}

    //! @name Operations
    //@{
    void Clear();
    void Update( const QString& name );
    virtual void Hide();
    virtual void Call( kernel::Property_ABC* const& property );
    //@}

public slots:
    //! @name Slots
    //@{
    void Show( bool );
    void OnValueChanged( int row, int col );
    //@}

private:
    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    virtual void focusOutEvent( QFocusEvent* );
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

    //! @name Types
    //@{
    typedef std::map< QString, unsigned int >  T_PropertyRows;
    typedef T_PropertyRows::const_iterator   CIT_PropertyRows;
    //@}

private:
    //! @name Member data
    //@{
    kernel::EditorFactory_ABC& factory_;
    TableItemDisplayer& itemDisplayer_;
    T_PropertyRows rows_;
    unsigned int row_;
    //@}
};

}

#endif // __PropertiesTable_h_
