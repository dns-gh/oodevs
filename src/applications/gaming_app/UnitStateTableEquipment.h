// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableEquipment_h_
#define __UnitStateTableEquipment_h_

#include "clients_gui/UnitStateTableEquipment.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Entity_ABC;
    enum E_EquipmentState;
    class Profile_ABC;
    class Time_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

class Equipments;
class StaticModel;

// =============================================================================
/** @class  UnitStateTableEquipment
    @brief  UnitStateTableEquipment
*/
// Created: ABR 2011-07-07
// =============================================================================
class UnitStateTableEquipment : public gui::UnitStateTableEquipment
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< Equipments >
{

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableEquipment( kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel,
                                      const kernel::Time_ABC& simulation, QWidget* parent, const kernel::Profile_ABC& profile,
                                      gui::DisplayExtractor& extractor );
    virtual ~UnitStateTableEquipment();
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
    virtual void NotifyUpdated( const Equipments& dotations );
    bool LineChanged( const QString& name, int& row, int size, E_EquipmentState state, const std::vector< unsigned int >& currentBreakdowns = std::vector< unsigned int >() ) const;
    std::vector< unsigned int > BreakdownIDToComboIndex( const QStringList* breakdowns, const std::vector< int >& breakdownIDs ) const;
    int CountLines( const QString& name, int firstRow, E_EquipmentState state ) const;
    unsigned int BreakdownComboIndexToId( const QStringList& breakdowns, unsigned int index ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, int >   T_Changes;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                      controllers_;
    const StaticModel&                        staticModel_;
    actions::ActionsModel&                    actionsModel_;
    const kernel::Time_ABC&                   simulation_;
    const kernel::Profile_ABC&                profile_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    mutable T_Changes                         rowsChanged_;
    //@}
};

#endif // __UnitStateTableEquipment_h_
