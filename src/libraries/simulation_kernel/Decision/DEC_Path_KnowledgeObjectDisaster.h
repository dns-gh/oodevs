// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeObjectDisaster_h_
#define __DEC_Path_KnowledgeObjectDisaster_h_

#include "DEC_Path_KnowledgeObject_ABC.h"

class DEC_Knowledge_Object;
class Extractor_ABC;
class MIL_Agent_ABC;
class PHY_ComposanteTypePion;
class TER_Localisation;

// =============================================================================
/** @class  DEC_Path_KnowledgeObjectDisaster
    @brief  DEC_Path_KnowledgeObjectDisaster
*/
// Created: JSR 2014-04-23
// =============================================================================
class DEC_Path_KnowledgeObjectDisaster : public DEC_Path_KnowledgeObject_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeObjectDisaster( const MIL_Agent_ABC& agent, const DEC_Knowledge_Object& knowledge );
    virtual ~DEC_Path_KnowledgeObjectDisaster();
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
    //! @name Helpers
    //@{
    float GetMaxValueOnPath( const MT_Vector2D& from, const MT_Vector2D& to ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< TER_Localisation > localisation_;
    std::set< const PHY_ComposanteTypePion* > composantes_;
    double maxTrafficability_;
    mutable double maxSpeedModifier_;
    std::vector< boost::shared_ptr< Extractor_ABC > > extractors_;
    //@}
};

#endif // __DEC_Path_KnowledgeObjectDisaster_h_
