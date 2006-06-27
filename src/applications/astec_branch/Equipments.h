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

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"

class Controller;
class Equipment;
class EquipmentType;
class DataDictionary;

// =============================================================================
/** @class  Equipments
    @brief  Equipments
*/
// Created: AGE 2006-02-13
// =============================================================================
class Equipments : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitDotations >
                 , public Resolver< Equipment >
{

public:
    //! @name Constructors/Destructor
    //@{
             Equipments( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, DataDictionary& dico );
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
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    void AddToDictionary( const Equipment& equipment );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< EquipmentType >& resolver_;
    DataDictionary& dico_;
    //@}
};

#endif // __Equipments_h_
