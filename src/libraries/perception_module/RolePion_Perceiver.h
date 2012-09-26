// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_ROLE_PION_PERCEIVER_H
#define SWORD_PERCEPTION_ROLE_PION_PERCEIVER_H

#include <boost/noncopyable.hpp>

class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class Perception_ABC;
    class PerceptionView;
    class PerceptionLevel;

// =============================================================================
// @class  RolePion_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class RolePion_Perceiver : private boost::noncopyable
{
public:
    //! @name Command operations
    //@{
    void ExecutePerceptions( const wrapper::View& model, const wrapper::View& entity ) const;
    //@}

    //! @name Hook operations
    //@{
    bool HasRadar( const wrapper::View& entity, size_t radarType ) const;
    const PerceptionLevel& ComputePointPerception( const wrapper::View& model, const wrapper::View& entity, const MT_Vector2D& vPoint ) const;
    const PerceptionLevel& ComputeKnowledgeObjectPerception( const wrapper::View& model, const wrapper::View& entity, const wrapper::View& object ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Target, typename Constructor, typename Computer >
    const PerceptionLevel& ComputePerception( const wrapper::View& model, const wrapper::View& entity, const Target& target, Constructor surfaceConstructor, Computer computer ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_ROLE_PION_PERCEIVER_H