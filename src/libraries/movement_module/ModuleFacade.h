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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <wrapper/Module.h>
#include <vector>
#include <map>

class KnowledgeCache;
class DEC_Knowledge_Object;
class MT_Vector2D;

namespace sword
{
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
class ModuleFacade : private sword::wrapper::Module
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

    std::vector< std::size_t > GetPoints( unsigned int entity ) const;
    const movement::PathPoint& GetPoint( std::size_t point ) const;
    void AddPathPoint( unsigned int entity, const boost::weak_ptr< movement::PathPoint >& point );
    void RemovePathPoint( unsigned int entity, std::size_t point );
    void RemovePathPoints( unsigned int entity );
    void RegisterPoint( const boost::weak_ptr< movement::PathPoint >& point );
    void UnregisterPoint( const boost::weak_ptr< movement::PathPoint >& point );
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned int, PathWalker* > paths_;
    std::map< unsigned int, std::vector< std::size_t > > pathPoints_;
    std::map< std::size_t, boost::weak_ptr< movement::PathPoint > > points_;
    //@}
};

}
}

#endif // movement_module_ModuleFacade_h
