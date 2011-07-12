// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PopulationKnowledgeParameter_h_
#define __MIL_PopulationKnowledgeParameter_h_

#include "MIL_BaseParameter.h"

class DEC_Knowledge_Object;
class DEC_KnowledgeResolver_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MIL_EntityManager_ABC;

// =============================================================================
/** @class  MIL_PopulationKnowledgeParameter
    @brief  MIL_PopulationKnowledgeParameter
*/
// Created: LDC 2009-06-04
// =============================================================================
class MIL_PopulationKnowledgeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationKnowledgeParameter();
    explicit MIL_PopulationKnowledgeParameter( DEC_Knowledge_Population* pKnowledge );
             MIL_PopulationKnowledgeParameter( const sword::CrowdKnowledgeId& asn, const DEC_KnowledgeResolver_ABC& resolver,
                                               const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_PopulationKnowledgeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToPopulationKnowledge( DEC_Knowledge_Population*& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@

private:
    //! @name Member data
    //@{
    DEC_Knowledge_Population* pKnowledgePopulation_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PopulationKnowledgeParameter )

#endif // __MIL_PopulationKnowledgeParameter_h_
