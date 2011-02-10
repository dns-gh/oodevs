// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
    const std::string& GetName() const;
    const std::string& GetProtection() const;
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
    std::string protection_;
    unsigned long id_;
    //@}
};

}

#endif // __EquipmentType_h_
