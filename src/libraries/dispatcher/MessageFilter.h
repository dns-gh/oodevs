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

#include "game_asn/Asn.h"

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
    bool IsRelevant( const ASN1T_MsgsOutSim& message );
    bool IsRelevant( const ASN1T_MsgsInClient& message );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsRelevant( const ASN1T_MsgUnitAttributes& message );
    bool IsRelevant( const ASN1T_MsgUnitKnowledgeUpdate& message );
    bool IsRelevant( const ASN1T_MsgObjectUpdate& message );
    bool IsRelevant( const ASN1T_MsgObjectKnowledgeUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationConcentrationUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationFluxUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationKnowledgeUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message );
    bool IsRelevant( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageFilter( const MessageFilter& );            //!< Copy constructor
    MessageFilter& operator=( const MessageFilter& ); //!< Assignement operator
    //@}
};

}

#endif // __MessageFilter_h_
