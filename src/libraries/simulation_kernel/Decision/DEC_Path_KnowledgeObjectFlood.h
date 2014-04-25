// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObjectFlood_h_
#define __DEC_Path_KnowledgeObjectFlood_h_

#include "DEC_Path_KnowledgeObject_ABC.h"
#include "simulation_terrain/TER_Localisation.h"

class DEC_Knowledge_Object;
enum E_CrossingHeight;

// =============================================================================
/** @class  DEC_Path_KnowledgeObjectFlood
    @brief  DEC_Path_KnowledgeObjectFlood
*/
// Created: JSR 2010-12-20
// =============================================================================
class DEC_Path_KnowledgeObjectFlood : public DEC_Path_KnowledgeObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObjectFlood( E_CrossingHeight crossingHeight, const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObjectFlood();
    //@}

    //! @name Operations
    //@{
    virtual double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& toTerrainType, const TerrainData& linkTerrainType, double weight ) const;
    virtual double GetCostOut() const;
    virtual double GetMaxTrafficability() const;
    virtual double GetAgentMaxSpeedMultiplier() const;
    virtual bool HasAgentMaxSpeedMultiplier() const;
    //@}

private:
    //! @name Member data
    //@{
    E_CrossingHeight crossingHeight_;
    std::vector< TER_Polygon > deepAreas_;
    std::vector< TER_Polygon > lowAreas_;
    TER_Localisation localisation_;
    double maxTrafficability_;
    //@}
};

#endif // __DEC_Path_KnowledgeObjectFlood_h_
