// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Effects/MIL_EffectManager.h $
// $Author: Nld $
// $Modtime: 21/02/05 11:35 $
// $Revision: 3 $
// $Workfile: MIL_EffectManager.h $
//
// *****************************************************************************

#ifndef __MIL_EffectManager_h_
#define __MIL_EffectManager_h_

#include <vector>

class MIL_Effect_ABC;
class MIL_Effect_IndirectFire;

// =============================================================================
// @class  MIL_EffectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EffectManager : private boost::noncopyable
{
private:
    //! @name Types
    //@{
    typedef std::vector< const MIL_Effect_IndirectFire* > T_FlyingShells;
    //@}

public:
             MIL_EffectManager();
    virtual ~MIL_EffectManager();

    //! @name Operations
    //@{
    void Register( MIL_Effect_ABC& effect );
    void Update();
    //@}

    //! @name Flying shells
    //@{
    void RegisterFlyingShell( const MIL_Effect_IndirectFire& effect );
    void UnregisterFlyingShell( const MIL_Effect_IndirectFire& effect );
    const T_FlyingShells& GetFlyingShells() const;
    //@}

    static MIL_EffectManager& GetEffectManager();

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Effect_ABC* > T_Effects;
    //@}

private:
    T_Effects      effects_;
    T_FlyingShells flyingShells_;
};

#endif // __MIL_EffectManager_h_
