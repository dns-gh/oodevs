// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameter_ResourceNetworkType_h_
#define __ActionParameter_ResourceNetworkType_h_

#include "Parameter.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class ResourceNetworkType;
}

namespace actions
{
    namespace parameters {

// =============================================================================
/** @class  ResourceNetworkType
    @brief  ResourceNetworkType
*/
// Created: ABR 2012-02-15
// =============================================================================
class ResourceNetworkType : public Parameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkType( const kernel::OrderParameter& parameter );
             ResourceNetworkType( const kernel::OrderParameter& parameter, const std::string name, const tools::StringResolver< kernel::ResourceNetworkType >& resolver );
    virtual ~ResourceNetworkType();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
    virtual void CommitTo( std::string& content ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ResourceNetworkType* type_;
    //@}
};

}
}

#endif // __ResourceNetworkType_h_
