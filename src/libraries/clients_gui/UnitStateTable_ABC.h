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

#include <boost/noncopyable.hpp>
#include <Qt3Support/q3table.h>
#include "clients_kernel/Tools.h"
#include "ComboTableItem.h"

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
class UnitStateTable_ABC : public Q3Table
                         , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTable_ABC( int numRows, int numCols, QWidget* parent, const char* name = 0 );
    virtual ~UnitStateTable_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual void Load( kernel::Entity_ABC& selected ) = 0;
    virtual void Commit( kernel::Entity_ABC& selected ) const = 0;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const = 0;
    //@}

    //! @name Operations
    //@{
    void Purge();
    void RecursiveLoad( kernel::Entity_ABC& selected );
    //@}

protected:
    //! @name Helpers
    //@{
    template< typename Enum >
    void Populate( Enum size, QStringList& content ) const;

    template< typename T >
    void AddCombo( unsigned int nRow, unsigned int nColumn, T value, const QStringList& contents, bool editable = true );

    template< typename T >
    T GetComboValue( unsigned int nRow, unsigned int nColumn ) const;

    template< typename T >
    T GetNumericValue( unsigned int nRow, unsigned int nColumn ) const;

    bool GetCheckboxValue( unsigned int nRow, unsigned int nColumn) const;
    //@}

protected:
    //! @name Data Members
    //@{
    bool readOnly_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::Populate
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template< typename Enum >
void UnitStateTable_ABC::Populate( Enum size, QStringList& content ) const
{
    for( unsigned int i = 0; i < unsigned int( size ); ++i )
        content << tools::ToString( static_cast< Enum >( i ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::AddCombo
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template< typename T >
void UnitStateTable_ABC::AddCombo( unsigned int nRow, unsigned int nColumn, T value, const QStringList& contents, bool editable /*= true*/ )
{
    ComboTableItem* combo = new ComboTableItem( this, contents, ( !editable || readOnly_ ) ? Q3TableItem::Never : Q3TableItem::WhenCurrent , static_cast< int >( value ) );
    setItem( nRow, nColumn, combo );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetComboValue
// Created: ABR 2011-07-20
// -----------------------------------------------------------------------------
template< typename T >
T UnitStateTable_ABC::GetComboValue( unsigned int nRow, unsigned int nColumn ) const
{
    return static_cast< T >( static_cast< ComboTableItem* >( item( nRow, nColumn ) )->CurrentItem() );
}

}

#endif // __gui_UnitStateTable_ABC_h_
