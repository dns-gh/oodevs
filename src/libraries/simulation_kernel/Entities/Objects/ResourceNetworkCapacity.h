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

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
}

namespace resource
{
    class NodeProperties;
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
    virtual ~ResourceNetworkCapacity();
    //@}

    //! @name Operations
    //@{
    void Update( xml::xistream& xis );
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    void RegisterNode( unsigned int id, bool urban );
    void SendState( MsgsSimToClient::MsgUrbanAttributes& message ) const;
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
