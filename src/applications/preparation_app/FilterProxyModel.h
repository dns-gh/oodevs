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

#include "preparation/ModelConsistencyChecker.h"
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
             FilterProxyModel( QWidget* parent );
    virtual ~FilterProxyModel();
    //@}

    //! @name Operations
    //@{
    void ToggleFilter( ModelConsistencyChecker::E_ConsistencyCheck type );
    //@}

private:
    //! @name Operations
    //@{
    virtual bool filterAcceptsRow( int row, const QModelIndex& parent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< ModelConsistencyChecker::E_ConsistencyCheck > T_Filters;
    //@}

private:
    //! @name Member data
    //@{
    T_Filters filters_;
    //@}
};

#endif // __FilterProxyModel_h_
