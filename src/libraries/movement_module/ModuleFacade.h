// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef movement_module_ModuleFacade_h
#define movement_module_ModuleFacade_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <wrapper/Hook.h>
#include <vector>
#include <map>

class KnowledgeCache;
class DEC_Knowledge_Object;
class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace movement
{
    class PathWalker;
    class Path_ABC;
    class PathPoint;

// =============================================================================
/** @class  ModuleFacade
    @brief  Module facade
*/
// Created: SLI 2011-12-13
// =============================================================================
class ModuleFacade : private sword::wrapper::Hooks, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleFacade( const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Register( unsigned int entity, PathWalker& walker );
    void Unregister( unsigned int entity );

    bool ComputeFutureObjectCollision( const wrapper::View& entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const;
    MT_Vector2D GetFuturePosition( const wrapper::View& entity, double rTime, bool bBoundOnPath ) const;
    bool IsMovingOnPath( unsigned int entity, const boost::shared_ptr< Path_ABC >& path ) const;

    void GetPoints( unsigned int entity, void(*callback)( const boost::shared_ptr< movement::PathPoint >& point, void* userData ), void* userData ) const;
    void AddPoints( unsigned int entity, const boost::shared_ptr< movement::PathPoint >& point );
    void RemovePoints( unsigned int entity, const boost::shared_ptr< movement::PathPoint >& point );
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, PathWalker* > paths_;
    std::map< unsigned int, std::vector< boost::shared_ptr< movement::PathPoint > > > points_;
    //@}
};

}
}

#endif // movement_module_ModuleFacade_h
