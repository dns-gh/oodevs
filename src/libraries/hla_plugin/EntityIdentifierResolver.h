// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASAGroup
//
// *****************************************************************************

#ifndef plugins_hla_EntityIdResolver_h
#define plugins_hla_EntityIdResolver_h

#include "EntityIdentifierResolver_ABC.h"
#include "rpr/EntityIdentifier.h"
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  EntityIdentifierResolver
    @brief  EntityIdentifierResolver
*/
// Created: AHC 2012-04-22
// =============================================================================
class EntityIdentifierResolver : public EntityIdentifierResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    EntityIdentifierResolver( unsigned short siteId, unsigned short applicationId );
    virtual ~EntityIdentifierResolver();
    //@}

    //! @name Operations
    //@{
    virtual void Create( const std::string& name, rpr::EntityIdentifier& id );
    virtual void Register( const rpr::EntityIdentifier& identifier, const std::string& name );
    virtual void Unregister( const rpr::EntityIdentifier& identifier );
    virtual void Unregister( const std::string& name );
    virtual const std::string& Resolve( const rpr::EntityIdentifier& identifier ) const;
    virtual const rpr::EntityIdentifier& Resolve( const std::string& name ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef boost::bimap< std::string, rpr::EntityIdentifier > T_IdMap;
    //@}

    //! @name Attributes
    //@{
    T_IdMap ids_;
    unsigned short siteId_;
    unsigned short applicationId_;
    unsigned short sequenceId_;
    //@}
};
}
}
#endif // plugins_hla_EntityIdResolver_h
