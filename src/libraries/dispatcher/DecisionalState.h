// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DecisionalState_h_
#define __DecisionalState_h_



namespace sword
{
    class DecisionalState;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Model_ABC;

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
    explicit DecisionalState( Model_ABC& model );
    virtual ~DecisionalState();
    //@}

    //! @name Operations
    //@{
    void Clear();
    void Update( const sword::DecisionalState& asnMsg );
    void Send( unsigned id, ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecisionalState( const DecisionalState& );            //!< Copy constructor
    DecisionalState& operator=( const DecisionalState& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > decisionalInfos_;
    Model_ABC& model_;
    //@}
};

}

#endif // __DecisionalState_h_
