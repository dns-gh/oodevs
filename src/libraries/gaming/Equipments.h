// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipments_h_
#define __Equipments_h_

#include "HierarchicExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class EquipmentType;
    class Entity_ABC;
    class Automat_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class UnitAttributes;
    class LogMaintenanceHandlingCreation;
    class LogMaintenanceHandlingUpdate;
}

class Equipment;

// =============================================================================
/** @class  Equipments
    @brief  Equipments
*/
// Created: AGE 2006-02-13
// =============================================================================
class Equipments : public kernel::Extension_ABC
                 , public HierarchicExtension_ABC
                 , public kernel::Updatable_ABC< sword::UnitAttributes >
                 , public tools::Resolver< Equipment >
                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Equipments( kernel::Entity_ABC& entity, kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, gui::PropertiesDictionary& dico
                       , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Equipments();
    //@}

    //! @name Operations
    //@{
    float GetTotalWeight() const;
    void CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message );
    void DeleteMaintenanceConsign( int id );
    void Update( const sword::LogMaintenanceHandlingUpdate& message );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void Update( const std::vector< Equipment >& differences );
    void AddToDictionary( const Equipment& equipment );
    void RemoveFromDictionary( const Equipment& equipment );
    void UpdateController();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    gui::PropertiesDictionary& dico_;
    const QString property_;
    //@}
};

#endif // __Equipments_h_
