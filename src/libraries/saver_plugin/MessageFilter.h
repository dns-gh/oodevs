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

namespace sword
{
    class SimToClient;
    class UnitAttributes;
    class UnitKnowledgeUpdate;
    class ObjectUpdate;
    class ObjectKnowledgeUpdate;
    class CrowdUpdate;
    class CrowdConcentrationUpdate;
    class CrowdFlowUpdate;
    class CrowdKnowledgeUpdate;
    class CrowdConcentrationKnowledgeUpdate;
    class CrowdFlowKnowledgeUpdate;
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
    bool IsRelevant( const sword::SimToClient& message );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsRelevant( const sword::UnitAttributes& message );
    bool IsRelevant( const sword::UnitKnowledgeUpdate& message );
    bool IsRelevant( const sword::ObjectUpdate& message );
    bool IsRelevant( const sword::ObjectKnowledgeUpdate& message );
    bool IsRelevant( const sword::CrowdUpdate& message );
    bool IsRelevant( const sword::CrowdConcentrationUpdate& message );
    bool IsRelevant( const sword::CrowdFlowUpdate& message );
    bool IsRelevant( const sword::CrowdKnowledgeUpdate& message );
    bool IsRelevant( const sword::CrowdConcentrationKnowledgeUpdate& message );
    bool IsRelevant( const sword::CrowdFlowKnowledgeUpdate& message );
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
