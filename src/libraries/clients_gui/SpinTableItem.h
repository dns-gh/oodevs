// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SpinTableItem_h_
#define __SpinTableItem_h_

#include <boost/noncopyable.hpp>
#include <QTable.h>

namespace gui
{

// =============================================================================
/** @class  SpinTableItem
    @brief  SpinTableItem
*/
// Created: JSR 2010-08-30
// =============================================================================
template< typename T >
class SpinTableItem : public QTableItem
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SpinTableItem( QTable* table, T minValue, T maxValue, T step = 1 );
    virtual ~SpinTableItem();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* createEditor() const;
    virtual void setContentFromEditor( QWidget* widget );
    void SetMinMaxValue( int minValue, int maxValue );
    //@}

private:
    //! @name Member data
    //@{
    T minValue_;
    T maxValue_;
    T step_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: SpinTableItem::SpinTableItem
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template< typename T >
SpinTableItem< T >::SpinTableItem( QTable* table, T minValue, T maxValue, T step /*= 1*/ )
    : QTableItem( table, QTableItem::WhenCurrent )
    , minValue_( minValue )
    , maxValue_( maxValue )
    , step_( step )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem destructor
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
template< typename T >
SpinTableItem< T >::~SpinTableItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinTableItem::SetMinMaxValue
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
template< typename T >
void SpinTableItem< T >::SetMinMaxValue( int minValue, int maxValue )
{
    minValue_ = minValue;
    maxValue_ = maxValue;
}


} // namespace gui

#endif // __SpinTableItem_h_
