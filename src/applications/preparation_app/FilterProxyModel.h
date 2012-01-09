// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterProxyModel_h_
#define __FilterProxyModel_h_

#include "preparation/ConsistencyErrorTypes.h"
#include <set>

// =============================================================================
/** @class  FilterProxyModel
    @brief  Filter proxy model
*/
// Created: LGY 2011-10-26
// =============================================================================
class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterProxyModel( QWidget* parent, bool( *IsError )( E_ConsistencyCheck ) );
    virtual ~FilterProxyModel();
    //@}

    //! @name Operations
    //@{
    void ToggleFilter( E_ConsistencyCheck type );
    void SetLevelFilter( bool warning, bool error );
    //@}

private:
    //! @name Operations
    //@{
    virtual bool filterAcceptsRow( int row, const QModelIndex& parent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< E_ConsistencyCheck > T_Filters;
    //@}

private:
    //! @name Member data
    //@{
    T_Filters filters_;
    bool (*IsError_)( E_ConsistencyCheck );
    bool warning_;
    bool error_;
    //@}
};

#endif // __FilterProxyModel_h_
