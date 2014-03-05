// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStocksListView_h_
#define __SupplyStocksListView_h_

namespace kernel
{
    class Entity_ABC;
}

#include "ResourcesListView_ABC.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Dotation.h"
#include "gaming/SupplyStates.h"

// =============================================================================
/** @class  SupplyStocksListView
    @brief  SupplyStocksListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyStocksListView : public ResourcesListView_ABC< SupplyStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStocksListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~SupplyStocksListView();
    //@}

public:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const SupplyStates& supplyStates );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void UpdateSelected( const kernel::Entity_ABC* entity );
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const tools::Resolver< Dotation >& dotations );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    void TotalizeStocks( const Dotation& dotations );
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned long, Dotation > dotations_;
    //@}
};

#endif // __SupplyStocksListView_h_
