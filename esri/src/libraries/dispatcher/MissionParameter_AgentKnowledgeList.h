// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_AgentKnowledgeList_h_
#define __MissionParameter_AgentKnowledgeList_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_AgentKnowledgeList
    @brief  MissionParameter_AgentKnowledgeList
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_AgentKnowledgeList : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_AgentKnowledgeList( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_AgentKnowledgeList();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_AgentKnowledgeList( const MissionParameter_AgentKnowledgeList& );            //!< Copy constructor
    MissionParameter_AgentKnowledgeList& operator=( const MissionParameter_AgentKnowledgeList& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< unsigned int > T_OIDVector;
    //@}

private:
    //! @name Member data
    //@{
    T_OIDVector agentKnowledges_;
    //@}
};

}

#endif // __MissionParameter_AgentKnowledgeList_h_
