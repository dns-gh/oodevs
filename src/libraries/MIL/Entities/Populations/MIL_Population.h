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

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Population : public PHY_Actor
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_Population( const MIL_PopulationType& type, uint nID, MIL_InputArchive& archive );
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
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Population( const MIL_Population& );            //!< Copy constructor
    MIL_Population& operator=( const MIL_Population& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_PopulationConcentration* > T_ConcentrationSet;
    typedef T_ConcentrationSet::iterator             IT_ConcentrationSet;
    typedef T_ConcentrationSet::const_iterator       CIT_ConcentrationSet;
    
    typedef std::set< MIL_PopulationFlow* > T_FlowSet;
    typedef T_FlowSet::iterator             IT_FlowSet;
    typedef T_FlowSet::const_iterator       CIT_FlowSet;
    //@}

private:
    //! @name Network
    //@{    
    void SendDestruction() const;    
    //@}

private:
    const MIL_PopulationType&     type_;
    const uint                    nID_;
    const MIL_Army*               pArmy_;
          std::string             strName_;
    const MIL_PopulationAttitude* pDefaultAttitude_;

          T_ConcentrationSet concentrations_;
          T_FlowSet          flows_;

          DEC_PopulationDecision*    pDecision_;
          MIL_PopulationOrderManager orderManager_;
};

#include "MIL_Population.inl"

#endif // __MIL_Population_h_
