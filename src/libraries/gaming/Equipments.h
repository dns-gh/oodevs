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

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class EquipmentType;
    class PropertiesDictionary;
    class Entity_ABC;
}

class Equipment;

// =============================================================================
/** @class  Equipments
    @brief  Equipments
*/
// Created: AGE 2006-02-13
// =============================================================================
class Equipments : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                 , public kernel::Resolver< Equipment >
{

public:
    //! @name Constructors/Destructor
    //@{
             Equipments( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, kernel::PropertiesDictionary& dico, kernel::Entity_ABC& holder );
    virtual ~Equipments();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Equipments( const Equipments& );            //!< Copy constructor
    Equipments& operator=( const Equipments& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    void AddToDictionary( const Equipment& equipment );
    void Update( const std::vector< Equipment >& differences );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::EquipmentType >& resolver_;
    kernel::PropertiesDictionary& dico_;
    kernel::Entity_ABC& holder_;
    //@}
};

#endif // __Equipments_h_
