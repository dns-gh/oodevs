// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentListener_ABC_h
#define plugins_hla_RemoteAgentListener_ABC_h

namespace plugins
{
namespace hla
{
    class Aggregate_ABC;

// =============================================================================
/** @class  RemoteAgentListener_ABC
    @brief  Remote agent listener definition
*/
// Created: SLI 2011-08-29
// =============================================================================
class RemoteAgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentListener_ABC() {}
    virtual ~RemoteAgentListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier, const Aggregate_ABC& aggregate ) = 0;
    virtual void Destroyed( const std::string& identifier, const Aggregate_ABC& aggregate ) = 0;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentListener_ABC_h
