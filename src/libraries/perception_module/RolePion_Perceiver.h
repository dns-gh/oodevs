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
    const PerceptionLevel& ComputePerception( const wrapper::View& entity, const MT_Vector2D& vPoint ) const;
    void ExecutePerceptions( const wrapper::View& model, const wrapper::View& entity ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_SensorMode
    {
        eNormal,    // la direction de détection est déterminée en fonction de la direction de déplacement
        eDirection, // la direction de détection est fixe
        ePoint      // la détection est fixées sur un point précis
    };

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