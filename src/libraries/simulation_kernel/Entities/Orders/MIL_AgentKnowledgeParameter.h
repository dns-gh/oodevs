// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentKnowledgeParameter_h_
#define __MIL_AgentKnowledgeParameter_h_

#include "MIL_BaseParameter.h"

class DEC_Knowledge_Agent;
class DEC_KnowledgeResolver_ABC;
class MIL_EntityManager_ABC;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_AgentKnowledgeParameter
    @brief  MIL_AgentKnowledgeParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AgentKnowledgeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentKnowledgeParameter();
    explicit MIL_AgentKnowledgeParameter( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
             MIL_AgentKnowledgeParameter( const sword::Id& asn, const DEC_KnowledgeResolver_ABC& resolver,
                                          const MIL_EntityManager_ABC& entityManager );
    virtual ~MIL_AgentKnowledgeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& value ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeAgent_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_AgentKnowledgeParameter )

#endif // __MIL_AgentKnowledgeParameter_h_
