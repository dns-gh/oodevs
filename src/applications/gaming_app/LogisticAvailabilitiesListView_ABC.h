// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAvailabilitiesListView_ABC_h_
#define __LogisticAvailabilitiesListView_ABC_h_

#include "gaming/Availability.h"
#include "gaming/Dotation.h"
#include "ResourcesListView_ABC.h"
#include "clients_kernel/Displayer_ABC.h"

#include "gaming/MaintenanceStates.h"
#include "gaming/MedicalStates.h"
#include "gaming/SupplyStates.h"

// =============================================================================
/** @class  LogisticAvailabilitiesListView_ABC
    @brief  LogisticAvailabilitiesListView_ABC
*/
// Created: SBO 2007-02-20
// =============================================================================
template< typename ConcreteDisplayer, typename Extension >
class LogisticAvailabilitiesListView_ABC : public ResourcesListView_ABC< ConcreteDisplayer, Extension >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticAvailabilitiesListView_ABC( QWidget* parent, ConcreteDisplayer& displayer, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
                 : ResourcesListView_ABC< ConcreteDisplayer, Extension >( parent, displayer, controllers, factory )
             {}
    virtual ~LogisticAvailabilitiesListView_ABC() {}
    //@}

    //! @name Operations
    //@{
    void Display( const Availability& availability, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
    {
        availability.Display( displayer );
    }

    void Display( const SupplyAvailability& availability, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
    {
        availability.Display( displayer );
    }

    void Display( const Dotation& dotation, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
    {
        displayer.Display( 0, dotation.type_ )
                 .Display( 0, dotation.quantity_ );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticAvailabilitiesListView_ABC( const LogisticAvailabilitiesListView_ABC& );            //!< Copy constructor
    LogisticAvailabilitiesListView_ABC& operator=( const LogisticAvailabilitiesListView_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Extension& a ) = 0;
    //@}
};

// =============================================================================
/** @class  MaintenanceAvailabilitiesListView_ABC
    @brief  MaintenanceAvailabilitiesListView_ABC
*/
// Created: SBO 2007-02-20
// =============================================================================
class MaintenanceAvailabilitiesListView_ABC : public LogisticAvailabilitiesListView_ABC< MaintenanceAvailabilitiesListView_ABC, MaintenanceStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceAvailabilitiesListView_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
                 : LogisticAvailabilitiesListView_ABC< MaintenanceAvailabilitiesListView_ABC, MaintenanceStates >( parent, *this, controllers, factory )
             {}
    virtual ~MaintenanceAvailabilitiesListView_ABC() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MaintenanceAvailabilitiesListView_ABC( const MaintenanceAvailabilitiesListView_ABC& );            //!< Copy constructor
    MaintenanceAvailabilitiesListView_ABC& operator=( const MaintenanceAvailabilitiesListView_ABC& ); //!< Assignment operator
    //@}
};

// =============================================================================
/** @class  MedicalAvailabilitiesListView_ABC
    @brief  MedicalAvailabilitiesListView_ABC
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalAvailabilitiesListView_ABC : public LogisticAvailabilitiesListView_ABC< MedicalAvailabilitiesListView_ABC, MedicalStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalAvailabilitiesListView_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
                 : LogisticAvailabilitiesListView_ABC< MedicalAvailabilitiesListView_ABC, MedicalStates >( parent, *this, controllers, factory )
             {}
    virtual ~MedicalAvailabilitiesListView_ABC() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalAvailabilitiesListView_ABC( const MedicalAvailabilitiesListView_ABC& );            //!< Copy constructor
    MedicalAvailabilitiesListView_ABC& operator=( const MedicalAvailabilitiesListView_ABC& ); //!< Assignment operator
    //@}
};

// =============================================================================
/** @class  SupplyAvailabilitiesListView_ABC
    @brief  SupplyAvailabilitiesListView_ABC
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyAvailabilitiesListView_ABC : public LogisticAvailabilitiesListView_ABC< SupplyAvailabilitiesListView_ABC, SupplyStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyAvailabilitiesListView_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
                 : LogisticAvailabilitiesListView_ABC< SupplyAvailabilitiesListView_ABC, SupplyStates >( parent, *this, controllers, factory )
             {}
    virtual ~SupplyAvailabilitiesListView_ABC() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyAvailabilitiesListView_ABC( const SupplyAvailabilitiesListView_ABC& );            //!< Copy constructor
    SupplyAvailabilitiesListView_ABC& operator=( const SupplyAvailabilitiesListView_ABC& ); //!< Assignment operator
    //@}
};

// =============================================================================
/** @class  QuotasAvailabilitiesListView_ABC
    @brief  QuotasAvailabilitiesListView_ABC
*/
// Created: SBO 2007-02-20
// =============================================================================
/*class QuotasAvailabilitiesListView_ABC : public LogisticAvailabilitiesListView_ABC< QuotasAvailabilitiesListView_ABC, Quotas >
{
public:
    //! @name Constructors/Destructor
    //@{
             QuotasAvailabilitiesListView_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
                 : LogisticAvailabilitiesListView_ABC< QuotasAvailabilitiesListView_ABC, Quotas >( parent, *this, controllers, factory )
             {}
    virtual ~QuotasAvailabilitiesListView_ABC() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    QuotasAvailabilitiesListView_ABC( const QuotasAvailabilitiesListView_ABC& );            //!< Copy constructor
    QuotasAvailabilitiesListView_ABC& operator=( const QuotasAvailabilitiesListView_ABC& ); //!< Assignment operator
    //@}
};
*/
#endif // __LogisticAvailabilitiesListView_ABC_h_
