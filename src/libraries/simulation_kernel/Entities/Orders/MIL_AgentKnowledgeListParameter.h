// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentKnowledgeListParameter_h_
#define __MIL_AgentKnowledgeListParameter_h_

#include "MIL_BaseParameter.h"
class DEC_Knowledge_Agent;
class DEC_KnowledgeResolver_ABC;

// =============================================================================
/** @class  MIL_AgentKnowledgeListParameter
    @brief  MIL_AgentKnowledgeListParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_AgentKnowledgeListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentKnowledgeListParameter( const ASN1T_UnitKnowledgeList& asn, const DEC_KnowledgeResolver_ABC& resolver );
    explicit MIL_AgentKnowledgeListParameter( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& knowledgeAgentList );
    virtual ~MIL_AgentKnowledgeListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& type ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const;
    virtual bool ToAgentKnowledgeList( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_AgentKnowledgeListParameter( const MIL_AgentKnowledgeListParameter& );            //!< Copy constructor
    MIL_AgentKnowledgeListParameter& operator=( const MIL_AgentKnowledgeListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > knowledgeAgentList_;
    //@}
};

#endif // __MIL_AgentKnowledgeListParameter_h_
