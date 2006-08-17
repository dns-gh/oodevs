// *****************************************************************************
//
// $Created: NLD 2004-04-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path_KnowledgeObject.h $
// $Author: Age $
// $Modtime: 10/06/05 14:40 $
// $Revision: 7 $
// $Workfile: DEC_Path_KnowledgeObject.h $
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObject_h_
#define __DEC_Path_KnowledgeObject_h_

#include "MIL.h"

#include "simulation_terrain/TER_Localisation.h"

class DEC_Knowledge_Object;
class DEC_Path;
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
    ~DEC_Path_KnowledgeObject();
    //@}

    //! @name Copy/Assignement
    //@{
    DEC_Path_KnowledgeObject( const DEC_Path_KnowledgeObject& );            //!< Copy constructor
    DEC_Path_KnowledgeObject& operator=( const DEC_Path_KnowledgeObject& ); //!< Assignement operator
    //@}

    //! @name Operations
    //@{
    MT_Float ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    //@}

private:
    //! @name Tools
    //@{
    void SetCost( MT_Float rCost );
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation localisation_;
    MT_Float         rCostIn_;
    MT_Float         rCostOut_;
    //@}
};

#endif // __DEC_Path_KnowledgeObject_h_
