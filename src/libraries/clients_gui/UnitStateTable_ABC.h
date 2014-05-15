// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateTable_ABC_h_
#define __gui_UnitStateTable_ABC_h_

#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Tools.h"
#include "CommonDelegate.h"
#include "RichTableView.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  UnitStateTable_ABC
    @brief  UnitStateTable_ABC
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTable_ABC : public RichTableView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTable_ABC( const QString& objectName,
                                 QWidget* parent,
                                 kernel::Controllers& controllers,
                                 const QStringList& horizontalHeaders );
    virtual ~UnitStateTable_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual void Load( kernel::Entity_ABC& selected ) = 0;
    virtual void Commit( kernel::Entity_ABC& selected ) const = 0;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const = 0;
    virtual bool IsReadOnlyForType( const std::string& typeName ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    void RecursiveLoad( kernel::Entity_ABC& entity, bool isSelectedEntity );
    void SetReadOnly( bool readOnly );
    bool IsReadOnly() const;
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename T >
    void AddItem( int row, int col, QString text, T value, Qt::ItemFlags flags = 0  );

    template< typename T >
    void SetData( int row, int col, QString text, T value );
    void SetEnabled( int row, int col, bool enabled );
    void SetCheckedState( int row, int col, bool checked );
    void SetColor( int row, int col, const QBrush& brush, int colorIndex );

    QVariant GetUserData( int row, int col ) const;
    QString GetDisplayData( int row, int col ) const;
    template< typename T >
    T GetEnumData( int row, int col ) const;
    bool GetCheckedState( int row, int col ) const;

    void InternalRecursiveLoad( kernel::Entity_ABC& entity, bool isSelectedEntity );
    //@}

signals:
    //! @name Signals
    //@{
    void RefreshFilters();
    //@}

protected:
    //! @name Data Members
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    bool aggregated_;
    //@}
};

#include "UnitStateTable_ABC.inl"

}

#endif // __gui_UnitStateTable_ABC_h_
