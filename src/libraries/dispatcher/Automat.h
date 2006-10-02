 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "AsnTypes.h"
#include "ModelRefsContainer.h"

namespace dispatcher
{
class Dispatcher;
class Side;
class KnowledgeGroup;
class Model;
class Agent;
class Publisher_ABC;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: NLD 2006-09-19
// =============================================================================
class Automat
{
public:
    //! @name Constructors/Destructor
    //@{
    Automat( Model& model, const ASN1T_MsgAutomateCreation& msg );
    ~Automat();
    //@}

    //! @name Accessors
    //@{
    unsigned long                GetID    () const;
    ModelRefsContainer< Agent >& GetAgents();
    //@}

    //! @name Main
    //@{
    void Update        ( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg );
    void Update        ( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

private:
          Model&                      model_;
    const unsigned long               nID_;
    const unsigned long               nType_; // XML reference - no resolved by dispatcher
    const std::string                 strName_;
          Side&                       side_;
          KnowledgeGroup*             pKnowledgeGroup_;
          ModelRefsContainer< Agent > agents_;

    //$$$ TMP
    unsigned long  nTC2_;
    unsigned long  nLogMaintenance_;
    unsigned long  nLogMedical_;
    unsigned long  nLogSupply_;
};

}

#include "Automat.inl"

#endif // __Automat_h_
