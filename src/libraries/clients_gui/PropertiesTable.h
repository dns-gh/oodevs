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
#include <qtable.h>

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
class PropertiesTable : public QTable
                      , public kernel::Displayer_ABC
                      , public kernel::Caller< kernel::Property_ABC* >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesTable( QWidget* parent );
    virtual ~PropertiesTable();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    virtual void Call( kernel::Property_ABC* const& property );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertiesTable( const PropertiesTable& );            //!< Copy constructor
    PropertiesTable& operator=( const PropertiesTable& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
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
