// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_Population_h_
#define __MIL_Population_h_

#include "MIL.h"

#include "Entities/Orders/Population/MIL_PopulationOrderManager.h"
#include "Entities/Actions/PHY_Actor.h"

class MIL_PopulationType;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_PopulationAttitude;
class MIL_Army;
class DEC_PopulationDecision;
class PHY_Volume;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Population : public PHY_Actor
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_Population( const MIL_PopulationType& type, uint nID, MIL_InputArchive& archive );
     MIL_Population();
    ~MIL_Population();
    //@}

    //! @name Accessors
    //@{
          uint                        GetID                () const;
    const MIL_PopulationType&         GetType              () const;
    const std::string&                GetName              () const;
          DEC_PopulationDecision&     GetDecision          () const;
    const MIL_PopulationOrderManager& GetOrderManager      () const;
          MIL_PopulationOrderManager& GetOrderManager      ();
          MT_Float                    GetMaxSpeed          () const;
          MT_Float                    GetDefaultFlowDensity() const;
    const MIL_PopulationAttitude&     GetDefaultAttitude   () const;
    const MIL_Army&                   GetArmy              () const;
          
    //@}

    //! @name Pion effects
    //@{
    MT_Float GetPionMaxSpeed( const MIL_PopulationAttitude& attitude, MT_Float rDensity, const PHY_Volume& pionVolume ) const;

    void     SetPionMaxSpeed  ( MT_Float rSpeed );
    void     ResetPionMaxSpeed();
    //@}

    //! @name Operations
    //@{
    void UpdateDecision();
    void UpdateState   ();
    void UpdateNetwork ();

    void Clean();
    //@}

    //! @name Actions
    //@{
    void Move( const MT_Vector2D& destination );
    //@}

    //! @name Tools
    //@{
    MIL_PopulationFlow&          CreateFlow      ( MIL_PopulationConcentration& concentration );
    MIL_PopulationConcentration& GetConcentration( const MT_Vector2D& position );
    //@}

    //! @name Network 
    //@{
    void OnReceiveMsgPopulationOrder( ASN1T_MsgPopulationOrder& msg, MIL_MOSContextID nCtx );
    void SendCreation               () const;
    void SendFullState              () const;
    void UpdateNetwork              () const;    

    void OnReceiveMsgPopulationMagicAction( ASN1T_MsgPopulationMagicAction& asnMsg, MIL_MOSContextID nCtx );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Population( const MIL_Population& );            //!< Copy constructor
    MIL_Population& operator=( const MIL_Population& ); //!< Assignement operator
    //@}

    //! @name Magic actions
    //@{
    ASN1T_EnumPopulationAttrErrorCode OnReceiveMsgMagicMove     ( ASN1T_MagicActionPopulationMoveTo& asn );
    ASN1T_EnumPopulationAttrErrorCode OnReceiveMsgDestroyAll    ();
    ASN1T_EnumPopulationAttrErrorCode OnReceiveMsgChangeAttitude( ASN1T_MagicActionPopulationChangeAttitude& asn );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_PopulationConcentration* > T_ConcentrationVector;
    typedef T_ConcentrationVector::iterator             IT_ConcentrationVector;
    typedef T_ConcentrationVector::const_iterator       CIT_ConcentrationVector;
    
    typedef std::vector< MIL_PopulationFlow* >          T_FlowVector;
    typedef T_FlowVector::iterator                      IT_FlowVector;
    typedef T_FlowVector::const_iterator                CIT_FlowVector;
    //@}

private:
    //! @name Network
    //@{    
    void SendDestruction() const;    
    //@}

private:
    const MIL_PopulationType*        pType_;
    const uint                       nID_;
    const MIL_Army*                  pArmy_;
          std::string                strName_;
    const MIL_PopulationAttitude*    pDefaultAttitude_;

          T_ConcentrationVector      concentrations_;
          T_FlowVector               flows_;

          T_ConcentrationVector      trashedConcentrations_;
          T_FlowVector               trashedFlows_;

          DEC_PopulationDecision*    pDecision_;
          MIL_PopulationOrderManager orderManager_;

          // Pion effects
          bool                       bPionMaxSpeedOverloaded_;
          MT_Float                   rOverloadedPionMaxSpeed_;
};

#include "MIL_Population.inl"

#endif // __MIL_Population_h_
