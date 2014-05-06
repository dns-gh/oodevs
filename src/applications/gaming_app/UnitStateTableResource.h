// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableResource_h_
#define __UnitStateTableResource_h_

#include "clients_gui/UnitStateTableResource.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace actions
{
    class ActionsModel;
}


namespace kernel
{
    class Controllers;
    class Dotations_ABC;
    class Equipments_ABC;
    class Entity_ABC;
    class Time_ABC;
}

class Dotation;
class StaticModel;

// =============================================================================
/** @class  UnitStateTableResource
    @brief  UnitStateTableResource
*/
// Created: ABR 2011-07-07
// =============================================================================
class UnitStateTableResource : public gui::UnitStateTableResource
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< kernel::Dotations_ABC >
                             , public tools::ElementObserver_ABC< kernel::Equipments_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableResource( kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel,
                                     const kernel::Time_ABC& simulation, QWidget* parent );
    virtual ~UnitStateTableResource();
    //@}

    //! @name Operations
    //@{
    virtual void Load( kernel::Entity_ABC& selected );
    virtual void Commit( kernel::Entity_ABC& selected ) const;
    virtual bool HasChanged( kernel::Entity_ABC& selected ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Dotations_ABC& dotations );
    virtual void NotifyUpdated( const kernel::Equipments_ABC& equipments );
    int HasDotationChanged( const Dotation& dotation ) const;
    void RecursiveMagicAction( kernel::Entity_ABC& entity, const QString& name, double percentage, unsigned int& quantity, kernel::Entity_ABC*& last ) const;
    void CreateMagicAction( unsigned int quantity, const Dotation& dotation, kernel::Entity_ABC* entity ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, int > T_Changes;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& staticModel_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    mutable T_Changes rowsChanged_;
    //@}
};

#endif // __UnitStateTableResource_h_
