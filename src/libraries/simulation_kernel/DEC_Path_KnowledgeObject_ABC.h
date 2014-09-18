// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObject_ABC_h_
#define __DEC_Path_KnowledgeObject_ABC_h_

class MT_Vector2D;
class TerrainData;

// =============================================================================
/** @class  DEC_Path_KnowledgeObject_ABC
    @brief  DEC_Path_KnowledgeObject_ABC
*/
// Created: JSR 2010-12-21
// =============================================================================
class DEC_Path_KnowledgeObject_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObject_ABC() {}
    virtual ~DEC_Path_KnowledgeObject_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double weight ) const = 0;
    virtual double GetCostOut() const = 0;
    virtual double GetMaxTrafficability() const = 0;
    virtual double ComputeAgentMaxSpeed( double speed, double maxSpeed ) const = 0;
    //@}
};

#endif // __DEC_Path_KnowledgeObject_ABC_h_
