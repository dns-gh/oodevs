// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyTransportersListView_h_
#define __SupplyTransportersListView_h_

#include "LogisticResourcesListView_ABC.h"

class SupplyStates;

// =============================================================================
/** @class  SupplyTransportersListView
    @brief  SupplyTransportersListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyTransportersListView : public LogisticResourcesListView_ABC< SupplyStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~SupplyTransportersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const SupplyStates& states ) const;
    //@}
};

#endif // __SupplyTransportersListView_h_
