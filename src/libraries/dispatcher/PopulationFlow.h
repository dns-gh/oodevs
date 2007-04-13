 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_h_
#define __PopulationFlow_h_

#include "game_asn/Asn.h"
#include "Localisation.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Publisher_ABC;
    class Model;
    class Population;

// =============================================================================
/** @class  PopulationFlow
    @brief  PopulationFlow
*/
// Created: NLD 2006-09-19
// =============================================================================
class PopulationFlow : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow( Model& model, Population& population, const ASN1T_MsgPopulationFluxCreation& msg );
    virtual ~PopulationFlow();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetID() const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationFluxCreation& msg );
    void Update        ( const ASN1T_MsgPopulationFluxUpdate& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    virtual void SendFullUpdate( Publisher_ABC& publisher ) const;
    virtual void CommitDestruction();
    void Accept        ( ModelVisitor_ABC& visitor );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFlow( const PopulationFlow& );            //!< Copy constructor
    PopulationFlow& operator=( const PopulationFlow& ); //!< Assignement operator
    //@}

private:
          Population&   population_;
    const unsigned long nID_;

    Localisation                 path_;
    Localisation                 flow_;
    unsigned int                 nDirection_;
    unsigned int                 nSpeed_;
    unsigned long                nNbrAliveHumans_;
    unsigned long                nNbrDeadHumans_;
    ASN1T_EnumPopulationAttitude nAttitude_;    
};

}

#include "PopulationFlow.inl"

#endif // __PopulationFlow_h_
