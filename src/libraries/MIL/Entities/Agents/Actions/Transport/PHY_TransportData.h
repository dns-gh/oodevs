// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Transport/PHY_TransportData.h $
// $Author: Jvt $
// $Modtime: 7/03/05 16:48 $
// $Revision: 4 $
// $Workfile: PHY_TransportData.h $
//
// *****************************************************************************

#ifndef __PHY_TransportData_h_
#define __PHY_TransportData_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_ComposanteState;
class NET_ASN_MsgUnitAttributes;
class MIL_NbcAgentType;

// =============================================================================
// @class  PHY_TransportData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_TransportData
{
    MT_COPYNOTALLOWED( PHY_TransportData );

public:
     PHY_TransportData( const MIL_AgentPion& transporter, DIA_Call_ABC& diaCall );
    ~PHY_TransportData();

    //! @name Accessors
    //@{
    MT_Float GetWeightToTransport() const;
    //@}

    //! @name Operations
    //@{
    void     Cancel     ();
    void     Damage     ( MT_Float rWeightDamaged, const PHY_ComposanteState& newState );
    void     Contaminate( MT_Float rWeightDamaged, const MIL_NbcAgentType& nbcAgentType );

    void     SetWeightToLoad( MT_Float rWeight );
    MT_Float NotifyLoaded   ( const MIL_Agent_ABC& transporter ); // Retourne le poids chargé
    void     NotifyUnloaded ();
    //@}

    //! @name Network
    //@{
    void SendFullState( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}
    
    //! @name Tools
    //@{
    static bool IsPionTransportable( const MIL_AgentPion& transporter, const MIL_AgentPion& toTransport, bool bTransportOnlyLoadable );
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eRunning,
        eFinished,
        eAlreadyTransporting,
        eImpossible
    };

    struct sTransportWeights
    {
        sTransportWeights();
        sTransportWeights( const sTransportWeights& rhs );
        sTransportWeights( MT_Float rTotalWeight );

        const MT_Float rTotalWeight_;
              MT_Float rRemainingWeight_;       
              MT_Float rTransportedWeight_;

    private:
        sTransportWeights& operator=( const sTransportWeights& rhs );
    };

    typedef std::map< MIL_AgentPion*, sTransportWeights > T_TransportedPionMap;
    typedef T_TransportedPionMap::iterator                IT_TransportedPionMap;
    typedef T_TransportedPionMap::const_iterator          CIT_TransportedPionMap;
    //@}

private:
    DIA_Variable_ABC&    diaReturnCode_;
    bool                 bTransportOnlyLoadable_;     
    T_TransportedPionMap transportedPions_;
    MT_Float             rWeightToLoad_;
};

#include "PHY_TransportData.inl"

#endif // __PHY_TransportData_h_
