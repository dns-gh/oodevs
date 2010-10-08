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

class DEC_Knowledge_Object;
class DEC_Agent_PathClass;
class MIL_Object_ABC;
class MT_Vector2D;
class TerrainData;

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
    double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    double GetCostOut() const;
    MIL_Object_ABC* GetObjectKnown() const;
    //@}

private:
    //! @name Member data
    //@{
    const DEC_Knowledge_Object* knowledge_;
    double rCostIn_;
    double rCostOut_;
    double rObstructionThreshold_;
    //@}
};

#include "DEC_Path_KnowledgeObject.inl"

#endif // __DEC_Path_KnowledgeObject_h_
