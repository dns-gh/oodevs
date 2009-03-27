// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttribute_ABC_h_
#define __ObjectAttribute_ABC_h_

class NET_ASN_MsgObjectCreation;

#include <HLA/Deserializer.h>
namespace hla 
{
    class AttributeIdentifier;
}
class HLA_UpdateFunctor;

class Object;
class DEC_Knowledge_Object;

namespace xml
{
    class xostream;
}

class ObjectAttribute_ABC
{
public:
    //! @name Constructor/destructor
    //@{
            ObjectAttribute_ABC() {}
    virtual ~ObjectAttribute_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

    //! @name Knowledge instance
    //@{    
    virtual void Instanciate( DEC_Knowledge_Object& /*object*/ ) const { /*NOTHING*/ }    
    //@}

    //! @name ASN Network update
    //@{     
    virtual void SendFullState( ASN1T_ObjectAttributes& /*asn*/ ) const {} //<! Send update to ASN client
    virtual void SendUpdate( ASN1T_ObjectAttributes& /*asn*/ ) const {} //<! Send update to ASN client
    virtual void OnUpdate( const ASN1T_ObjectAttributes& /*asn*/ ) {} //<! Update from ASN msg
    virtual void Serialize( HLA_UpdateFunctor& /*functor*/ ) const {} //<! Send update to HLA client
    virtual void Deserialize( const hla::AttributeIdentifier& /*attributeID*/, hla::Deserializer /*deserializer*/ ) {} //<! Update from HLA
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const {} 
    //@}
};

#endif
