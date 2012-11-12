// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __rpr_EntityType_h_
#define __rpr_EntityType_h_

#include <string>

namespace rpr
{
// =============================================================================
/** @class  EntityType
    @brief  EntityTypeStruct complex data type
*/
// Created: AGE 2008-02-21
// =============================================================================
class EntityType
{
public:
    enum RPRKind
    {
        KIND_OTHER = 0,
        PLATFORM = 1,
        AMMUNITION = 2,
        LIFEFORM = 3,
        ENVIRONMENTAL = 4,
        CULTURAL_FEATURE = 5,
        SUPPLY = 6,
        RADIO = 7,
        EXPENDABLE = 8,
        SENSOR_EMITTER = 9,
        VRF_AGGREGATE = 11 // VRForces specific
    };
    enum RPRDomain
    {
        OTHER = 0,
        LAND = 1,
        AIR = 2,
        SURFACE = 3,
        SUBSURFACE = 4,
        SPACE = 5
    };
    //! @name Constructors/Destructor
    //@{
             EntityType();
    explicit EntityType( const std::string& value );
    virtual ~EntityType();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << entityKind_  << domain_   << countryCode_ << category_
                << subcategory_ << specific_ << extra_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> entityKind_  >> domain_   >> countryCode_ >> category_
                >> subcategory_ >> specific_ >> extra_;
    }
    
    std::string str() const;
    unsigned char Domain() const { return domain_; }
    unsigned char Kind() const { return entityKind_; }
    //@}

    //! @name Operators
    //@{
    friend EntityType operator-( const EntityType& lhs, const EntityType& rhs )
    {
        EntityType result;
        result.entityKind_  = static_cast< unsigned char >(  std::abs( lhs.entityKind_  - rhs.entityKind_ ) );
        result.domain_      = static_cast< unsigned char >(  std::abs( lhs.domain_      - rhs.domain_ ) );
        result.countryCode_ = static_cast< unsigned short >( std::abs( lhs.countryCode_ - rhs.countryCode_ ) );
        result.category_    = static_cast< unsigned char >(  std::abs( lhs.category_    - rhs.category_ ) );
        result.subcategory_ = static_cast< unsigned char >(  std::abs( lhs.subcategory_ - rhs.subcategory_ ) );
        result.specific_    = static_cast< unsigned char >(  std::abs( lhs.specific_    - rhs.specific_ ) );
        result.extra_       = static_cast< unsigned char >(  std::abs( lhs.extra_       - rhs.extra_ ) );
        return result;
    }

    bool operator<=( const EntityType& rhs ) const;
    bool operator==( const EntityType& rhs ) const;
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

}

#endif // __rpr_EntityType_h_
