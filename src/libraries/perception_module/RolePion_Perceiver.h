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

#include <vector>
#include <boost/noncopyable.hpp>

class PerceptionCoupDeSonde;
class PerceptionRecoPoint;
class PerceptionRecoLocalisation;
class PerceptionRecoObjects;
class PerceptionRecoSurveillance;
class PerceptionRecoUrbanBlock;
class PerceptionRadar;
class PerceptionAlat;
class PerceptionFlyingShell;
class MT_Vector2D;
class DEC_Knowledge_Object;

namespace core
{
    class Model_ABC;
}

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
    //! @name Constructors/Destructor
    //@{
             RolePion_Perceiver();
    virtual ~RolePion_Perceiver();
    //@}

    //! @name Operations
    //@{
    bool HasRadar( const wrapper::View& entity, size_t radarType ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& entity, const MT_Vector2D& vPoint ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& entity, const DEC_Knowledge_Object& object ) const;
    void ExecutePerceptions( const wrapper::View& model, const wrapper::View& entity ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Target, typename Constructor >
    const PerceptionLevel& ComputePerception( const wrapper::View& entity, const Target& target, Constructor surfaceConstructor ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Perception_ABC* > T_PerceptionVector;
    typedef T_PerceptionVector::iterator IT_PerceptionVector;
    typedef T_PerceptionVector::const_iterator CIT_PerceptionVector;
    //@}

private:
    //! @name Member data
    //@{
    //PerceptionCoupDeSonde*      pPerceptionCoupDeSonde_;
    //PerceptionRecoPoint*        pPerceptionRecoPoint_;
    //PerceptionRecoLocalisation* pPerceptionRecoLocalisation_;
    //PerceptionRecoUrbanBlock*   pPerceptionRecoUrbanBlock_;
    //PerceptionRecoObjects*      pPerceptionRecoObjects_;
    //PerceptionRecoSurveillance* pPerceptionSurveillance_;
    //PerceptionRadar*            pPerceptionRadar_;
    //PerceptionAlat*             pPerceptionAlat_;
    //PerceptionFlyingShell*      pPerceptionFlyingShell_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_ROLE_PION_PERCEIVER_H