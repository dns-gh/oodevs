// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ParameterList_h_
#define __ParameterList_h_

#include "Parameter_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Displayer_ABC.h"
#include "Parameter.h"
#include <xeumeuleu/xml.hpp>

namespace google
{
    namespace protobuf
    {
        template< typename T > class RepeatedPtrField;
    }
}
namespace Common
{
    class MsgMissionParameter_Value;
}
namespace kernel
{
    class Entity_ABC;
}
namespace actions {

    class ParameterFactory_ABC;

    namespace parameters {

// =============================================================================
/** @class  ParameterList
    @brief  ParameterList
*/
// Created: JSR 2010-04-15
// =============================================================================
class ParameterList : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ParameterList( const kernel::OrderParameter& parameter );
             ParameterList( const kernel::OrderParameter& parameter, const ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >& list, const actions::ParameterFactory_ABC& factory, const kernel::Entity_ABC& entity );
    virtual ~ParameterList();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetType() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void CommitTo( Common::MsgMissionParameter_Value& message ) const;

    ParameterList& AddList      ( const std::string& name );

    void AddBool      ( const std::string& name, bool value );
    void AddIdentifier( const std::string& name, unsigned int id );
    void AddNumeric   ( const std::string& name, float value );
    void AddQuantity  ( const std::string& name, int value );
    void AddString    ( const std::string& name, const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    //@}
};

    }
}

#endif // __ParameterList_h_
