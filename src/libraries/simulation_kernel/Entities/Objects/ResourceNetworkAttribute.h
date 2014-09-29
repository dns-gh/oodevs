// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkAttribute_h_
#define __ResourceNetworkAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyPassThrough.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

namespace resource
{
    class NodeProperties;
}

// =============================================================================
/** @class  ResourceNetworkAttribute
    @brief  ResourceNetworkAttribute
*/
// Created: JSR 2010-08-31
// =============================================================================
class ResourceNetworkAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkAttribute();
    explicit ResourceNetworkAttribute( MIL_Object_ABC& object );
             ResourceNetworkAttribute( xml::xistream& xis, MIL_Object_ABC& object );
    virtual ~ResourceNetworkAttribute();
    //@}

    //! @name Copy/Assignment
    //@{
    ResourceNetworkAttribute& operator=( const ResourceNetworkAttribute& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    bool Update( const ResourceNetworkAttribute& rhs );
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkAttribute( const ResourceNetworkAttribute& );            //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< resource::NodeProperties> nodeProperties_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ResourceNetworkAttribute )

#endif // __ResourceNetworkAttribute_h_
