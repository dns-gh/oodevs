// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UnitStateTableCrew_h_
#define __UnitStateTableCrew_h_

#include "clients_gui/UnitStateTableCrew.h"
#include "clients_kernel/SafePointer.h"
#include "ENT/ENT_Tr.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Entity_ABC;
    class OrderParameter;
    class Profile_ABC;
    class Time_ABC;
}

class StaticModel;
class Troops;

// =============================================================================
/** @class  UnitStateTableCrew
    @brief  UnitStateTableCrew
*/
// Created: ABR 2011-07-07
// =============================================================================
class UnitStateTableCrew : public gui::UnitStateTableCrew
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< Troops >
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableCrew( const QString& objectName,
                                 kernel::Controllers& controllers,
                                 const StaticModel& staticModel,
                                 actions::ActionsModel& actionsModel,
                                 const kernel::Time_ABC& simulation,
                                 QWidget* parent );
    virtual ~UnitStateTableCrew();
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
    virtual void NotifyUpdated( const Troops& troops );
    const kernel::OrderParameter& GetEnumOrderParameter( unsigned int column ) const;
    template< typename T >
    void PopulateEnumOrderParameters( const std::string& name, const std::string& type, unsigned int column, T size );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, kernel::OrderParameter* > T_OrderParameterMap;
    typedef T_OrderParameterMap::iterator                    IT_OrderParameterMap;
    typedef T_OrderParameterMap::const_iterator             CIT_OrderParameterMap;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                      controllers_;
    const StaticModel&                        staticModel_;
    actions::ActionsModel&                    actionsModel_;
    const kernel::Time_ABC&                   simulation_;
    T_OrderParameterMap                       orderParameterMap_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::PopulateEnumOrderParameters
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
template< typename Enum >
void UnitStateTableCrew::PopulateEnumOrderParameters( const std::string& name, const std::string& type, unsigned int column, Enum size )
{
    assert( orderParameterMap_.find( column ) == orderParameterMap_.end() );
    orderParameterMap_[ column ] = new kernel::OrderParameter( name, type, false );
    for( unsigned int i = 0; i < unsigned int( size ); ++i )
        orderParameterMap_[ column ]->AddValue( i, tools::ToString( static_cast< Enum >( i ), ENT_Tr::eToSim ).toStdString() );
}

#endif // __UnitStateTableCrew_h_
