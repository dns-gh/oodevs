// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Hierarchies_h_
#define __Hierarchies_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Controller;
class Agent;
class Gtia;

// =============================================================================
/** @class  Hierarchies
    @brief  Hierarchies
*/
// Created: AGE 2006-02-14
// =============================================================================
class Hierarchies : public Extension_ABC
                  , public Updatable_ABC< ASN1T_MsgChangeAutomateAck >
                  , public Updatable_ABC< ASN1T_MsgChangeAutomate >
                  , public Updatable_ABC< ASN1T_MsgChangeGroupeConnaissanceAck >
                  , public Updatable_ABC< ASN1T_MsgAutomateCreation >
                  , public Updatable_ABC< ASN1T_MsgPionCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             Hierarchies( Controller& controller, const Resolver_ABC< Agent >& agentResolver, const Resolver_ABC< Gtia >& gtiaResolver );
    virtual ~Hierarchies();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Hierarchies( const Hierarchies& );            //!< Copy constructor
    Hierarchies& operator=( const Hierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgChangeAutomateAck& message );
    virtual void Update( const ASN1T_MsgChangeAutomate& message );
    virtual void Update( const ASN1T_MsgChangeGroupeConnaissanceAck& message );
    virtual void Update( const ASN1T_MsgAutomateCreation& message );
    virtual void Update( const ASN1T_MsgPionCreation& message );

    void ChangeGtia( unsigned long id );
    void ChangeGtia( const Gtia& gtia );
    void ChangeSuperior( unsigned long id );
    void AddChild   ( Hierarchies& child );
    void RemoveChild( Hierarchies& child );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Hierarchies* > T_Hierarchies;
    typedef T_Hierarchies::iterator    IT_Hierarchies;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& agentResolver_;
    const Resolver_ABC< Gtia >&  gtiaResolver_;

    const Gtia*   gtia_;
    Hierarchies*  superior_;
    T_Hierarchies children_;
    //@}
};

#endif // __Hierarchies_h_
