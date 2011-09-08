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

namespace sword
{
    class ObjectAttributes;
    class MissionParameter_Value;
}

class MIL_Object_ABC;
class DEC_Knowledge_Object;
struct ASN1T_ObjectAttributes;

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  ObjectAttribute_ABC
    @brief  ObjectAttribute_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
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
    template< typename Archive >
    void serialize( Archive&, const unsigned int )
    {
        // NOTHING
    }
    //@}

    //! @name Knowledge instance
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& /*object*/ ) const { /*NOTHING*/ }
    virtual void Register( MIL_Object_ABC& /*object*/ ) const { /*NOTHING*/ }
    //@}

    //! @name ASN Network update
    //@{
    virtual void SendFullState( sword::ObjectAttributes& /*asn*/ ) const {}             //<! Send update to asn client
    virtual bool SendUpdate( sword::ObjectAttributes& /*asn*/ ) const { return false; } //<! Send update to asn client
    virtual void OnUpdate( const sword::MissionParameter_Value& /*attribute*/ ) {}      //<! Update from asn msg
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const {}
    //@}
};

#endif
