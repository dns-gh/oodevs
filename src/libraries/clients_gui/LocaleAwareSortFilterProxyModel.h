// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LocaleAwareSortFilterProxyModel_h
#define LocaleAwareSortFilterProxyModel_h

#include <QtGui/QSortFilterProxyModel>

namespace gui
{

// =============================================================================
/** @class LocaleAwareSortFilterProxyModel
    @brief LocaleAwareSortFilterProxyModel
*/
// Created: ABR 2013-08-29
// =============================================================================
class LocaleAwareSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LocaleAwareSortFilterProxyModel( QObject* parent = 0 )
                 : QSortFilterProxyModel( parent )
    {
        // NOTHING
    }
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool lessThan( const QModelIndex& left, const QModelIndex& right ) const
    {
        QString txt1 = sourceModel()->data( left ).toString();
        QString txt2 = sourceModel()->data( right ).toString();
        return txt1.localeAwareCompare( txt2 ) > 0;
    }
    //@}
};

}

#endif // LocaleAwareSortFilterProxyModel_h
