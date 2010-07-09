// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DecisionalState_h_
#define __DecisionalState_h_



namespace MsgsSimToClient
{
    class MsgDecisionalState;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  DecisionalState
    @brief  DecisionalState
*/
// Created: ZEBRE 2007-06-21
// =============================================================================
class DecisionalState
{
public:
    //! @name Constructors/Destructor
    //@{
             DecisionalState();
    virtual ~DecisionalState();
    //@}

    //! @name Operations
    //@{
    void Clear();
    void Update( const MsgsSimToClient::MsgDecisionalState& asnMsg );
    void Send( unsigned id, ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > decisionalInfos_;
    //@}
};

}

#endif // __DecisionalState_h_
