// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EntityType_h_
#define __EntityType_h_

namespace plugins
{
namespace hla
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
    //! @name Constructors/Destructor
    //@{
             EntityType();
    explicit EntityType( const std::string& value);
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
    //@}

    //! @name Operators
    //@{
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
}

#endif // __EntityType_h_
