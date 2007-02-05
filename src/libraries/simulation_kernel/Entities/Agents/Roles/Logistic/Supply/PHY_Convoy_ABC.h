// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_Convoy_ABC.h $
// $Author: Jvt $
// $Modtime: 1/04/05 11:44 $
// $Revision: 5 $
// $Workfile: PHY_Convoy_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_Convoy_ABC_h_
#define __PHY_Convoy_ABC_h_

#include "MIL.h"

#include "MT_Tools/MT_InterpolatedFunction.h"

class PHY_SupplyConsign_ABC;
class PHY_ComposantePion;
class MIL_AgentPion;
class MIL_AgentTypePion;
class MIL_PionMissionType;
class MIL_Automate;
class MIL_AutomateLOG;
class PHY_Conveyor;

// =============================================================================
// @class  PHY_Convoy_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Convoy_ABC
{
    MT_COPYNOTALLOWED( PHY_Convoy_ABC )

public:
    //! @name 
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();
    //@}

public:
             PHY_Convoy_ABC( PHY_SupplyConsign_ABC& consign );
             PHY_Convoy_ABC();
    virtual ~PHY_Convoy_ABC();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    bool     ReserveTransporters();
    MT_Float ModifySpeed        ( MT_Float rSpeed ) const;
    //@}                

    //! @name Accessors
    //@{
          MIL_AutomateLOG& GetSupplyingAutomate() const;
          MIL_AutomateLOG& GetConvoyingAutomate() const;
    const MIL_Automate&    GetSuppliedAutomate () const;
          uint             GetFormingTime      () const;
          uint             GetLoadingTime      () const;
          uint             GetUnloadingTime    () const;
    //@}

    //! @name Events
    //@{
    void EmptyOut               ();
    void NotifyConveyorDestroyed( PHY_ComposantePion& composante );
    //@}
 
public:
    //! @name Types
    //@{
    typedef std::map< PHY_ComposantePion*, PHY_Conveyor* > T_ConveyorMap;
    typedef T_ConveyorMap::const_iterator                  CIT_ConveyorMap;
    //@}

protected:
    PHY_SupplyConsign_ABC* pConsign_;
    T_ConveyorMap          conveyors_;

private:
    //! @name Tools
    //@{
    void UnlockConvoy();
    //@}

private:
    //! @name Initialization
    //@{
    static void InitializeConvoyUnitType( MIL_InputArchive& archive );
    static void InitializeConvoyMission ( MIL_InputArchive& archive );

    static void InitializeInterpolatedTime ( MIL_InputArchive& archive, const std::string& strTagName, MT_InterpolatedFunction< MT_Float >& data );
    static void InitializeSpeedModificators( MIL_InputArchive& archive );
    //@}

protected:
    static       MT_InterpolatedFunction< MT_Float > formingTime_;
    static       MT_InterpolatedFunction< MT_Float > loadingTime_;
    static       MT_InterpolatedFunction< MT_Float > unloadingTime_;
    static       MT_InterpolatedFunction< MT_Float > coefSpeedModificator_;

    static const MIL_AgentTypePion*                  pConvoyAgentType_;
    static const MIL_PionMissionType*                pConvoyMissionType_;
};

#include "PHY_Convoy_ABC.inl"

#endif // __PHY_Convoy_ABC_h_
