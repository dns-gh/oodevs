// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionList_h_
#define __ExtensionList_h_

#include "ParameterList.h"

namespace sword
{
    class Extension;
}

namespace actions
{
    namespace parameters {

// =============================================================================
/** @class  ExtensionList
    @brief  ExtensionList
*/
// Created: ABR 2011-05-13
// =============================================================================
class ExtensionList : public ParameterList
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionList( const kernel::OrderParameter& parameter, const sword::Extension& message );
    explicit ExtensionList( const kernel::OrderParameter& parameter );
    virtual ~ExtensionList();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void AddExtension( const std::string& key, const std::string& value );
    //@}

private:
    //! @name Helpers
    //@{
    void CommitTo( sword::Extension& message ) const;
    //@}
};
}
}

#endif // __ExtensionList_h_
