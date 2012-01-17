// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObject_h_
#define __DEC_Path_KnowledgeObject_h_

#include "DEC_Path_KnowledgeObject_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

class DEC_Knowledge_Object;
class DEC_Agent_PathClass;
class DEC_Population_PathClass;

// =============================================================================
// Created: NLD 2004-04-06
// =============================================================================
class DEC_Path_KnowledgeObject : public DEC_Path_KnowledgeObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObject( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Object& knowledge );
             DEC_Path_KnowledgeObject( const DEC_Population_PathClass& pathClass, const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObject();
    //@}

    //! @name Operations
    //@{
    virtual double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double weight ) const;
    virtual double GetCostOut() const;
    virtual double GetMaxTrafficability() const;
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation localisation_;
    TER_Localisation scaledLocalisation_;
    TER_Localisation realLocalisation_;
    double rCostIn_;
    double rCostOut_;
    double rObstructionThreshold_;
    double rMaxTrafficability_;
    //@}
};

#include "DEC_Path_KnowledgeObject.inl"

#endif // __DEC_Path_KnowledgeObject_h_
