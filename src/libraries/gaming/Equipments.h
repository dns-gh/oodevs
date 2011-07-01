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
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controller;
    class EquipmentType;
    class PropertiesDictionary;
    class Entity_ABC;
    class Automat_ABC;
}

namespace sword
{
    class UnitAttributes;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             Equipments( kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, kernel::PropertiesDictionary& dico
                       , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Equipments();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Equipments( const Equipments& );            //!< Copy constructor
    Equipments& operator=( const Equipments& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void Update( const std::vector< Equipment >& differences );
    void AddToDictionary( const Equipment& equipment );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    kernel::PropertiesDictionary& dico_;
    //@}
};

#endif // __Equipments_h_
