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
    explicit MIL_AgentKnowledgeParameter( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
             MIL_AgentKnowledgeParameter( const ASN1T_UnitKnowledge& asn, const DEC_KnowledgeResolver_ABC& resolver );
    virtual ~MIL_AgentKnowledgeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToAgentKnowledge( ASN1T_UnitKnowledge& asn ) const;
    virtual bool ToAgentKnowledge( boost::shared_ptr< DEC_Knowledge_Agent >& value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AgentKnowledgeParameter( const MIL_AgentKnowledgeParameter& );            //!< Copy constructor
    MIL_AgentKnowledgeParameter& operator=( const MIL_AgentKnowledgeParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeAgent_;
    //@}
};

#endif // __MIL_AgentKnowledgeParameter_h_
