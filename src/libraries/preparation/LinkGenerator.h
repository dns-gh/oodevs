// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LinkGenerator_h_
#define __LinkGenerator_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  LinkGenerator
    @brief  Link generator
*/
// Created: LGY 2011-10-12
// =============================================================================
class LinkGenerator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LinkGenerator();
    virtual ~LinkGenerator();
    //@}

    //! @name Operations
    //@{
    void GenerateFromAutomat( const kernel::Entity_ABC& automat );
    void GenerateFromFormation( const kernel::Entity_ABC& formation );
    void Generate( const kernel::Entity_ABC& entity );
    void RemoveFromAutomat( const kernel::Entity_ABC& automat );
    void RemoveFromFormation( const kernel::Entity_ABC& formation );
    void Remove( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Helpers
    //@{
    void AddLogisticSuperior( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior );
    void RemoveLogisticSuperior( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior );
    void CreateLink( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& base, std::function< bool( const kernel::Entity_ABC& ) > fun );
    void DeleteLink( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& base );
    //@}
};

#endif // __LinkGenerator_h_
