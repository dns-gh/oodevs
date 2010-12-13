// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkCapacity_h_
#define __ResourceNetworkCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include <boost/serialization/export.hpp>

namespace google
{
namespace protobuf
{
    template< typename T > class RepeatedPtrField;
}
}

namespace sword
{
    class MsgMissionParameter_Value;
    class ObjectAttributes;
    class UrbanAttributes;
}

namespace resource
{
    class NodeProperties;
}

namespace urban
{
    class ResourceNetworkAttribute;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ResourceNetworkCapacity
    @brief  ResourceNetworkCapacity
*/
// Created: JSR 2010-08-12
// =============================================================================
class ResourceNetworkCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkCapacity();
    explicit ResourceNetworkCapacity( xml::xistream& xis );
    explicit ResourceNetworkCapacity( const urban::ResourceNetworkAttribute& urbanAttribute );
    virtual ~ResourceNetworkCapacity();
    //@}

    //! @name Checkpoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update( xml::xistream& xis, const MIL_Object_ABC& object );
    void Update( const google::protobuf::RepeatedPtrField< sword::MsgMissionParameter_Value >& list );

    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;

    void RegisterNode( unsigned int id, bool urban );
    void SendState( sword::UrbanAttributes& message );
    void SendState( sword::ObjectAttributes& asn ) const;
    void SetModifier( unsigned int modifier ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkCapacity( const ResourceNetworkCapacity& );            //!< Copy constructor
    ResourceNetworkCapacity& operator=( const ResourceNetworkCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    resource::NodeProperties* nodeProperties_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ResourceNetworkCapacity )

#endif // __ResourceNetworkCapacity_h_
