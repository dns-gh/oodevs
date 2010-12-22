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
#include <geometry/Types.h>
#include "simulation_terrain/TER_Localisation.h"

class DEC_Knowledge_Object;
class DEC_Agent_PathClass;

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
             DEC_Path_KnowledgeObjectFlood( const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObjectFlood();
    //@}

    //! @name Operations
    //@{
    virtual double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    virtual double GetCostOut() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Path_KnowledgeObjectFlood( const DEC_Path_KnowledgeObjectFlood& );            //!< Copy constructor
    DEC_Path_KnowledgeObjectFlood& operator=( const DEC_Path_KnowledgeObjectFlood& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    //std::vector< geometry::Polygon2f > deepAreas_;
    //std::vector< geometry::Polygon2f > lowAreas_;
    std::vector< TER_Polygon > deepAreas_;
    std::vector< TER_Polygon > lowAreas_;
    TER_Localisation localisation_;
    //@}
};

#endif // __DEC_Path_KnowledgeObjectFlood_h_
