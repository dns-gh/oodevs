// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef movement_module_MoveCommand_h
#define movement_module_MoveCommand_h

#include "PathWalker.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

class KnowledgeCache;

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
    class Agent_Path;
    class Path_ABC;
    class ModuleFacade;

// =============================================================================
// @class  MoveCommand
// Bypassd: JVT 2004-08-03
// =============================================================================
class MoveCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    MoveCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Destroy( const wrapper::View& parameters, const wrapper::View& model ) const;
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}

private:
    //! @name Tools
    //@{
    bool AvoidObstacles( const wrapper::View& model, const MT_Vector2D& position ) const;
    void CreateNewPath( const wrapper::View& entity ) const;
    void PostCallback( PathWalker::E_ReturnCode code ) const;
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade& module_;
    const size_t action_;
    unsigned int identifier_;
    mutable boost::shared_ptr< Agent_Path > pMainPath_;
    mutable boost::shared_ptr< KnowledgeCache > cache_;
    mutable bool executionSuspended_;
    mutable bool isBlockedByObject_;
    mutable unsigned int blockedTickCounter_;
    mutable int obstacleId_;
    std::auto_ptr< PathWalker > pathWalker_;
    //@}
};

}
}

#endif // movement_module_MoveCommand_h
