// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h $
// $Author: Jvt $
// $Modtime: 4/05/05 14:41 $
// $Revision: 5 $
// $Workfile: PHY_RoleAction_Loading.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Loading_h_
#define __PHY_RoleAction_Loading_h_

#include "MT_Tools/MT_Role_ABC.h"

class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RoleAction_Loading
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Loading : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Loading )

public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_Loading RoleInterface;
    //@}

public:
    explicit PHY_RoleAction_Loading( MT_RoleContainer& role );
             PHY_RoleAction_Loading();
    virtual ~PHY_RoleAction_Loading();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Operations
    //@{
    int  GetInitialReturnCode() const;
    int  GetFinalReturnCode  () const;

    int  Load          ();
    void LoadSuspended ();

    int  Unload         ();
    void UnloadSuspended();

    void CheckConsistency  (); // Appelé quand l'état des composantes d'un pion change
    void ForceUnloadedState();
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    bool     IsLoaded        () const;
    MT_Float GetLoadingTime  () const;
    MT_Float GetUnloadingTime() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,
        eEnd,
        eErrorNoCarriers,
        eErrorNoCarried,
        eErrorLoadUnloadInSameTime
    };

    enum E_State
    {
        eLoading,
        eUnloading,
        eNothing
    };
    //@}

private:
    //! @name Tools
    //@{
    MT_Float ComputeLoadingTime  () const;
    MT_Float ComputeUnloadingTime() const;

    void SetLoadedState  ();
    void SetUnloadedState();
    //@}
    
private:
    //! @name Member data
    //@{
    E_State nState_;
    bool    bIsLoaded_;
    uint    nEndTimeStep_; // Load or unload
    bool    bHasChanged_;
    bool    bHasBeenUpdated_; // Le trigger Load / Unload ou RecoverCarriers a été appelé durant le pas de temps
    //@}
};

#endif // __PHY_RoleAction_Loading_h_
