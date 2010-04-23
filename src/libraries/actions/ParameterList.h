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
#include <xeumeuleu/xml.h>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ParameterList
    @brief  ParameterList
*/
// Created: JSR 2010-04-15
// =============================================================================
class ParameterList : public Parameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParameterList( const kernel::OrderParameter& parameter );
    virtual ~ParameterList();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetType() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;

    ParameterList& AddList      ( const std::string& name );

    void AddBool      ( const std::string& name, bool value );
    void AddIdentifier( const std::string& name, unsigned int id );
    void AddNumeric   ( const std::string& name, float value );
    void AddQuantity  ( const std::string& name, int value );
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
