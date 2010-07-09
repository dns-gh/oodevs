// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageFilter_h_
#define __MessageFilter_h_


namespace MsgsSimToClient
{
    class MsgSimToClient;
    class MsgUnitAttributes;
    class MsgUnitKnowledgeUpdate;
    class MsgObjectUpdate;
    class MsgObjectKnowledgeUpdate;
    class MsgPopulationUpdate;
    class MsgPopulationConcentrationUpdate;
    class MsgPopulationFlowUpdate;
    class MsgPopulationKnowledgeUpdate;
    class MsgPopulationConcentrationKnowledgeUpdate;
    class MsgPopulationFlowKnowledgeUpdate;
}

namespace dispatcher
{

// =============================================================================
/** @class  MessageFilter
    @brief  MessageFilter
*/
// Created: AGE 2007-04-12
// =============================================================================
class MessageFilter
{
public:
    //! @name Constructors/Destructor
    //@{
             MessageFilter();
    virtual ~MessageFilter();
    //@}

    //! @name Operations
    //@{
    bool IsRelevant( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsRelevant( const MsgsSimToClient::MsgUnitAttributes& message );
    bool IsRelevant( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgObjectUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationConcentrationUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationFlowUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageFilter( const MessageFilter& );            //!< Copy constructor
    MessageFilter& operator=( const MessageFilter& ); //!< Assignment operator
    //@}
};

}

#endif // __MessageFilter_h_
