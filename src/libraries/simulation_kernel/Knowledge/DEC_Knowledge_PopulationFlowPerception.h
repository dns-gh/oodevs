// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlowPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationFlowPerception_h_
#define __DEC_Knowledge_PopulationFlowPerception_h_

#include "MIL.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class DEC_Knowledge_PopulationPerception;
class MIL_PopulationFlow;
class MIL_PopulationAttitude;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlowPerception : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlowPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationFlow& flowPerceived );
             DEC_Knowledge_PopulationFlowPerception();
    virtual ~DEC_Knowledge_PopulationFlowPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const PHY_PerceptionLevel& level, const T_PointVector& shape );
    bool Clean();
    bool IsIdentified();
    bool IsPerceived () const; // Perception au tick courant
    //@}

    //! @name Accessors
    //@{
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
    const MIL_PopulationFlow& GetFlowPerceived() const;
    const DEC_Knowledge_PopulationPerception& GetKnowledge() const;
    unsigned int GetNbrAliveHumans() const;
    unsigned int GetNbrDeadHumans() const;
    const MIL_PopulationAttitude& GetAttitude() const;
    const T_PointVector& GetShape() const;
    const MT_Vector2D& GetDirection() const;
    double GetSpeed() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork() const;
    void SendStateToNewClient( unsigned int nCtx = 0) const;
    //@}

private:
    //! @name Member data
    //@{
    const DEC_Knowledge_PopulationPerception* pPopulationKnowledge_;
    MIL_PopulationFlow* pPopulationFlowPerceived_;
    T_PointVector shape_;
    T_PointVector previousShape_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationFlowPerception )

#endif // __DEC_Knowledge_PopulationFlowPerception_h_
