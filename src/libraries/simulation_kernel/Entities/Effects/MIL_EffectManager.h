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

class MIL_Effect_ABC;
class MIL_Effect_IndirectFire;

// =============================================================================
// @class  MIL_EffectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_EffectManager : private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Effect_IndirectFire* > T_FlyingShellSet;
    typedef T_FlyingShellSet::const_iterator           CIT_FlyingShellSet;
    //@}

public:
     MIL_EffectManager();
    ~MIL_EffectManager();

    //! @name Operations
    //@{
    void Register( MIL_Effect_ABC& effect );
    void Update();
    //@}    

    //! @name Flying shells
    //@{
          void              RegisterFlyingShell  ( const MIL_Effect_IndirectFire& effect );
          void              UnregisterFlyingShell( const MIL_Effect_IndirectFire& effect );
    const T_FlyingShellSet& GetFlyingShells      () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Effect_ABC* > T_EffectSet;
    typedef T_EffectSet::iterator       IT_EffectSet;
    //@}

private:
    T_EffectSet      effects_;
    T_FlyingShellSet flyingShells_;
};

#endif // __MIL_EffectManager_h_
