// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ConsistencyDialog_h_
#define __ADN_ConsistencyDialog_h_

#include <boost/noncopyable.hpp>
#include "clients_gui/ConsistencyDialog_ABC.h"
#include "ADN_ConsistencyErrorTypes.h"
#include "ADN_ConsistencyChecker.h"

// =============================================================================
/** @class  ADN_ConsistencyDialog
    @brief  ADN_ConsistencyDialog
*/
// Created: ABR 2012-06-06
// =============================================================================
class ADN_ConsistencyDialog : public gui::ConsistencyDialog< E_ConsistencyCheck, ADN_ConsistencyChecker::T_Items >
{
    Q_OBJECT

    typedef gui::ConsistencyDialog< E_ConsistencyCheck, ADN_ConsistencyChecker::T_Items > T_Parent;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ConsistencyDialog( QWidget* parent );
    virtual ~ADN_ConsistencyDialog();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnSelectionChanged( const QModelIndex& );
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateDataModel();
    void DoUpdateDataModel( const ADN_ConsistencyChecker::T_ConsistencyErrors& errors );
    //@}

signals:
    //! @name Signals
    //@{
    void GoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );
    //@}
};

#endif // __ADN_ConsistencyDialog_h_
