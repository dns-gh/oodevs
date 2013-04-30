// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_FilterProxyModel_h_
#define __gui_FilterProxyModel_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  FilterProxyModel
    @brief  FilterProxyModel
*/
// Created: ABR 2012-06-06
// =============================================================================
template< typename EnumError >
class FilterProxyModel : public QSortFilterProxyModel
{
public:
    //! @name Types
    //@{
    typedef EnumError( *T_TypeToMaskConverter)( EnumError );
    typedef std::set< EnumError > T_Filters;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FilterProxyModel( T_TypeToMaskConverter convertTypeToMask );
    virtual ~FilterProxyModel();
    //@}

    //! @name Operations
    //@{
    void ToggleFilter( EnumError type );
    void SetLevelFilter( bool warning, bool error );
    //@}

private:
    //! @name Operations
    //@{
    virtual bool filterAcceptsRow( int row, const QModelIndex& parent ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Filters             filters_;
    bool                  warning_;
    bool                  error_;
    T_TypeToMaskConverter convertTypeToMask_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: FilterProxyModel constructor
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
template< typename EnumError >
FilterProxyModel< EnumError >::FilterProxyModel( T_TypeToMaskConverter convertTypeToMask )
    : QSortFilterProxyModel()
    , convertTypeToMask_( convertTypeToMask )
    , warning_          ( true )
    , error_            ( true )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel destructor
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
template< typename EnumError >
FilterProxyModel< EnumError >::~FilterProxyModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::ToggleFilter
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
template< typename EnumError >
void FilterProxyModel< EnumError >::ToggleFilter( EnumError type )
{
    if( filters_.find( type ) != filters_.end() )
        filters_.erase( type );
    else
        filters_.insert( type );
    beginResetModel();
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::SetLevelFilter
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
template< typename EnumError >
void FilterProxyModel< EnumError >::SetLevelFilter( bool warning, bool error )
{
    warning_ = warning;
    error_ = error;
    beginResetModel();
}

// -----------------------------------------------------------------------------
// Name: FilterProxyModel::filterAcceptsRow
// Created: LGY 2011-10-26
// -----------------------------------------------------------------------------
template< typename EnumError >
bool FilterProxyModel< EnumError >::filterAcceptsRow( int row, const QModelIndex& parent ) const
{
    QModelIndex index = sourceModel()->index( row, 0, parent );
    EnumError type = static_cast< EnumError >( index.data( Qt::UserRole + 1 ).toInt() );
    if( filters_.find( convertTypeToMask_( type ) ) == filters_.end() )
        return false;
    bool isError = index.data( Qt::UserRole + 2 ).toBool();
    return ( warning_ && !isError ) || ( error_ && isError );
}

} //! namespace gui

#endif // __gui_FilterProxyModel_h_
