// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AttackController_h_
#define __MIL_AttackController_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

class MIL_Effect_AttackPopulation;
class MIL_PopulationElement_ABC;
class TER_PopulationConcentrationManager;
class MIL_EffectManager;

// =============================================================================
/** @class  MIL_AttackController
    @brief  MIL attack controller
*/
// Created: LGY 2012-02-10
// =============================================================================
class MIL_AttackController : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AttackController( const TER_PopulationConcentrationManager& concentrationManager,
                                   MIL_EffectManager& effectManager );
    virtual ~MIL_AttackController();
    //@}

    //! @name Operations
    //@{
    void Attack( MIL_PopulationElement_ABC& attacking, float intensity );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< MIL_Effect_AttackPopulation > T_Effect;
    typedef std::map< unsigned int, T_Effect >               T_Effects;
    typedef T_Effects::iterator                             IT_Effects;
    typedef T_Effects::const_iterator                      CIT_Effects;
    //@}

private:
    //! @name Member data
    //@{
    T_Effects effects_;
    const TER_PopulationConcentrationManager& concentrationManager_;
    MIL_EffectManager& effectManager_;
    //@}
};

#endif // __MIL_AttackController_h_
