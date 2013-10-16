// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLEEXTENDER_ABC_H_
#define SWORD_ROLEEXTENDER_ABC_H_

#include <tools/TypeIdentificators.h>
#include <tools/TypeMap.h>
#include <tools/Extendable.h>

class MIL_AgentPion;
class Role_ABC;

namespace sword
{
// Allows storing role factories for later use
// The registered factories can be applied either:
// - on a per role basis, using the GetFactory method
// - as a whole using the ResgisterRoles method
class RoleExtender_ABC
{
public:
    //! @name Public types
    //@{
    struct RoleFactory_ABC : private boost::noncopyable
    {
        virtual ~RoleFactory_ABC() {}
        virtual void RegisterRole( MIL_AgentPion& ) = 0;
    };
    //@}

    //! @name Constructors/destructor
    //@{
    virtual ~RoleExtender_ABC() {}
    //@}

    //! @name Methods
    //@{
    virtual void RegisterRoles( MIL_AgentPion& ) = 0;

    // Retrieve the role factory for the given Role
    // If successful, the factory is removed from the internal list (hence, if called twice, the second call will return an empty shared_ptr)
    template< typename Role >
    boost::shared_ptr< RoleFactory_ABC > GetFactory( )
    {
        boost::shared_ptr< RoleFactory_ABC > retval;
        T_RoleFactories::iterator it = roleFactories_.find( Identificators::GetTypeId< typename Role::RoleInterface >() );
        if( it != roleFactories_.end() )
        {
            retval = it->second;
            roleFactories_.erase( it );
        }
        return retval;
    }
    //@}

protected:
    //! @name Private types
    //@{
    typedef tools::TypeIdentificators< tools::Extendable< Role_ABC > > Identificators;
    typedef std::map< std::size_t, boost::shared_ptr< RoleFactory_ABC > > T_RoleFactories;
    //@}

    //! @name Member data
    //@{
    T_RoleFactories roleFactories_;
    //@}
};

}

#endif // SWORD_ROLEEXTENDER_ABC_H_
