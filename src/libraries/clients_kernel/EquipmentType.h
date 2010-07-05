// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentType_h_
#define __EquipmentType_h_

#include "tools/Resolver.h"

namespace xml { class xistream; };

namespace kernel
{
    class WeaponSystemType;

// =============================================================================
/** @class  EquipmentType
    @brief  EquipmentType
*/
// Created: AGE 2006-02-21
// =============================================================================
class EquipmentType : public  tools::Resolver< WeaponSystemType, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             EquipmentType( xml::xistream& xis, const  tools::Resolver_ABC< WeaponSystemType, std::string >& weapons );
    virtual ~EquipmentType();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    unsigned long GetId() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EquipmentType( const EquipmentType& );            //!< Copy constructor
    EquipmentType& operator=( const EquipmentType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadWeaponSystem( xml::xistream& xis, const  tools::Resolver_ABC< WeaponSystemType, std::string >& weapons );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    unsigned long id_;
    //@}
};

}

#endif // __EquipmentType_h_
