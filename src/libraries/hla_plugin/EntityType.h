// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityType_h_
#define __EntityType_h_

// =============================================================================
/** @class  EntityType
    @brief  EntityTypeStruct complex data type
*/
// Created: AGE 2008-02-21
// =============================================================================
class EntityType
{
public:
    //! @name Types
    //@{
    static const int other = 0;
    enum E_Kind
    {
        platform        = 1,
        lifeForm        = 3,
    };
    enum E_Domain
    {
        land    = 1,
        air     = 2,
    };
    enum E_Country
    {
        australia = 13,
        france    = 71,
        us        = 225
    };
    enum E_Category
    {
        tank                    = 1, airDefenseFighter   = 1, dismountedInfantry = 1,
        armoredFightingVehicle  = 2, attackStrikeFighter = 2,
        armoredUtilityVehicle   = 3, bomber              = 3, civilian           = 3,
        artillery               = 4,
                                     attackHelicopter    = 20,
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             EntityType();
    virtual ~EntityType();
    //@}

    //! @name Modifiers
    //@{
    void SetKind    ( E_Kind kind );
    void SetDomain  ( E_Domain domain );
    void SetCountry ( E_Country country );
    void SetCategory( E_Category category );
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << entityKind_  << domain_   << countryCode_ << category_ 
                << subcategory_ << specific_ << extra_;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned char entityKind_;
    unsigned char domain_;
    unsigned short countryCode_;
    unsigned char category_;
    unsigned char subcategory_;
    unsigned char specific_;
    unsigned char extra_;
    //@}
};

#endif // __EntityType_h_
