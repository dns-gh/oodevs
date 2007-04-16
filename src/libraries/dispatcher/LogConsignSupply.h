 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignSupply_h_
#define __LogConsignSupply_h_

#include "game_asn/Asn.h"
#include "LogSupplyDotation.h"
#include "ModelsContainer.h"
#include "Entity_ABC.h"

namespace dispatcher
{
class Dispatcher;
class Agent;
class Automat;
class Model;
class Publisher_ABC;

// =============================================================================
/** @class  LogConsignSupply
    @brief  LogConsignSupply
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignSupply : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignSupply( Model& model, const ASN1T_MsgLogRavitaillementTraitementCreation& msg );
    virtual ~LogConsignSupply();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgLogRavitaillementTraitementCreation& msg );
    void Update        ( const ASN1T_MsgLogRavitaillementTraitementUpdate& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    void CommitDestruction();
    void Accept( ModelVisitor_ABC& visitor );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignSupply( const LogConsignSupply& );            //!< Copy constructor
    LogConsignSupply& operator=( const LogConsignSupply& ); //!< Assignement operator
    //@}

private:
          Model&        model_;
    const unsigned long nID_;
    const Automat&      automat_;
    const unsigned long nTickCreation_;
          
    Automat*                                  pTreatingAutomat_;
    Automat*                                  pConvoyingAutomat_;
    Agent*                                    pConvoy_;
    ASN1T_EnumLogRavitaillementTraitementEtat nState_;
    ModelsContainer< LogSupplyDotation >      dotations_;
};

}

#include "LogConsignSupply.inl"

#endif // __LogConsignSupply_h_
