// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"

namespace hla 
{
    class Deserializer;    
}

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class BypassAttribute 
    : public ObjectAttribute_ABC
    , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BypassAttribute();
    explicit BypassAttribute( const ASN1T_ObjectAttributes& asn );
    virtual ~BypassAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );    
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void OnUpdate( const ASN1T_ObjectAttributes& asn );
    void Serialize( HLA_UpdateFunctor& functor ) const;
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Accessors
    //@{
    void     Update( MT_Float progress );
    MT_Float GetState() const;
    bool     IsBypassed() const;
    //@}

    //! @name Copy
    //@{
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{    
    MT_Float rBypass_;
    //@}
};

#endif // __BypassAttribute_h_
