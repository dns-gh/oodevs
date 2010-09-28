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
    class MsgCrowdUpdate;
    class MsgCrowdConcentrationUpdate;
    class MsgCrowdFlowUpdate;
    class MsgCrowdKnowledgeUpdate;
    class MsgCrowdConcentrationKnowledgeUpdate;
    class MsgCrowdFlowKnowledgeUpdate;
}

namespace plugins
{
namespace saver
{
// =============================================================================
/** @class  MessageFilter
    @brief  Message filter
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
    bool IsRelevant( const MsgsSimToClient::MsgCrowdUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgCrowdConcentrationUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgCrowdFlowUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgCrowdKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate& message );
    bool IsRelevant( const MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageFilter( const MessageFilter& );            //!< Copy constructor
    MessageFilter& operator=( const MessageFilter& ); //!< Assignment operator
    //@}
};

}
}

#endif // __MessageFilter_h_
