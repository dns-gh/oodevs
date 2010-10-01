// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_Object_ABC_h_
#define __HLA_Object_ABC_h_

namespace hla
{
    class AttributeIdentifier;
    class UpdateFunctor_ABC;
    class Deserializer;
    class ObjectIdentifier;
}

// =============================================================================
/** @class  HLA_Object_ABC
    @brief  Shared object definition
*/
// Created: AGE 2004-11-30
// =============================================================================
class HLA_Object_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             HLA_Object_ABC() {}
    virtual ~HLA_Object_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Construct( double rDeltaPercentage ) = 0;
    virtual void Destroy( double rDeltaPercentage ) = 0;
    virtual void Mine( double rDeltaPercentage ) = 0;
    virtual void Demine( double rDeltaPercentage ) = 0;
    virtual void Bypass( double rDeltaPercentage ) = 0;
//    virtual void Prepare() = 0;
    virtual void Activate() = 0;
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer ) = 0;
    virtual void Serialize( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const = 0;
    virtual void Destroy() = 0;
    virtual bool HideObject() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual hla::ObjectIdentifier GetId() const = 0;
    //@}
};

#endif // __HLA_Object_ABC_h_
