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

#include "simulation_terrain/TER_Localisation.h"

class DEC_Knowledge_Object;
class DEC_Agent_PathClass;

// =============================================================================
// Created: NLD 2004-04-06
// =============================================================================
class DEC_Path_KnowledgeObject
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObject( const DEC_Agent_PathClass& pathClass, const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObject();
    //@}

    //! @name Copy/Assignment
    //@{
    DEC_Path_KnowledgeObject( const DEC_Path_KnowledgeObject& );            //!< Copy constructor
    DEC_Path_KnowledgeObject& operator=( const DEC_Path_KnowledgeObject& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    MT_Float GetCostOut () const;
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation localisation_;
    MT_Float rCostIn_;
    MT_Float rCostOut_;
    MT_Float rObstructionThreshold_;
    //@}
};

#include "DEC_Path_KnowledgeObject.inl"

#endif // __DEC_Path_KnowledgeObject_h_
