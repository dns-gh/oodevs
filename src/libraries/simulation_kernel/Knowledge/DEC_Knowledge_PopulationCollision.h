// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationCollision.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:33 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_PopulationCollision.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationCollision_h_
#define __DEC_Knowledge_PopulationCollision_h_

#include "DEC_Knowledge_ABC.h"
#include <tools/Set.h>

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_Agent_ABC;
class DEC_Knowledge_Population;
class MT_Vector2D;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationCollision : public DEC_Knowledge_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationCollision( const MIL_Agent_ABC& agentColliding, MIL_Population& population );
    explicit DEC_Knowledge_PopulationCollision( MIL_Population& population );
             DEC_Knowledge_PopulationCollision ();
    virtual ~DEC_Knowledge_PopulationCollision();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( MIL_PopulationFlow& flow );
    void Update( MIL_PopulationConcentration& concentration );
    bool Clean();
    void PublishKnowledges( DEC_Knowledge_Population& knowledge ) const;
    //@}

    //! @name Operations
    //@{
    double GetMaxPopulationDensity() const;
    double GetPionMaxSpeed() const;
    double GetPionReloadingTimeFactor() const;
    void GetPositions( std::vector< MT_Vector2D >& positions ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation() const;
    //@}

private:
    //! @name Data Members
    //@{
    const MIL_Agent_ABC* pAgentColliding_;
    MIL_Population* pPopulation_;
    tools::Set< MIL_PopulationFlow* > flows_;
    tools::Set< MIL_PopulationConcentration* > concentrations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationCollision )

#endif // __DEC_Knowledge_PopulationCollision_h_
