// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObjectBurnSurface_h_
#define __DEC_Path_KnowledgeObjectBurnSurface_h_

#include "DEC_Path_KnowledgeObject_ABC.h"

class DEC_Knowledge_Object;
class BurnSurfaceAttribute;

// =============================================================================
/** @class  DEC_Path_KnowledgeObjectBurnSurface
    @brief  Allow to find path between or around fire cells
*/
// Created: BCI 2011-01-05
// =============================================================================
class DEC_Path_KnowledgeObjectBurnSurface : public DEC_Path_KnowledgeObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObjectBurnSurface( const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObjectBurnSurface();
    //@}

    //! @name Operations
    //@{
    virtual double ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const;
    virtual double GetCostOut() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DEC_Path_KnowledgeObjectBurnSurface( const DEC_Path_KnowledgeObjectBurnSurface& );            //!< Copy constructor
    DEC_Path_KnowledgeObjectBurnSurface& operator=( const DEC_Path_KnowledgeObjectBurnSurface& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const BurnSurfaceAttribute* pBurnSurfaceAttribute_;
    //@}
};

#endif // __DEC_Path_KnowledgeObjectBurnSurface_h_
