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

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Network/NetworkBufferedPercentageValue.h"
#include <boost/serialization/export.hpp>

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
class BypassAttribute : public ObjectAttribute_ABC
                      , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BypassAttribute();
    explicit BypassAttribute( const Common::MsgMissionParameter_Value& attributes );
    virtual ~BypassAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( Common::ObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::ObjectAttributes& asn ) const;
    virtual void OnUpdate( const Common::MsgMissionParameter_Value& attribute );
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Accessors
    //@{
    void Update    ( double progress );
    bool IsBypassed() const;
    //@}

    //! @name Copy
    //@{
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    bool Update( const BypassAttribute& rhs );
    //@}

private:
    //! @name Member data
    //@{
    mutable NetworkBufferedPercentageValue< double > bypassPercentage_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BypassAttribute )

#endif // __BypassAttribute_h_
