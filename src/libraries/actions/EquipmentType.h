// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameterEquipmentType_h_
#define __ActionParameterEquipmentType_h_

#include "Parameter.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class EquipmentType;
}

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  EquipmentType
    @brief  Equipment type
*/
// Created: MCO 2014-04-28
// =============================================================================
class EquipmentType : public Parameter< std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EquipmentType( const kernel::OrderParameter& parameter );
             EquipmentType( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::EquipmentType >& resolver );
    virtual ~EquipmentType();
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
    const kernel::EquipmentType* type_;
    //@}
};

}
}

#endif // __ActionParameterEquipmentType_h_
