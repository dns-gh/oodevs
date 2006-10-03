 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "AsnTypes.h"
#include "ModelsContainer.h"

namespace dispatcher
{
class Publisher_ABC;
class Model;
class Side;
class PopulationConcentration;
class PopulationFlow;

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population
{
public:
    //! @name Constructors/Destructor
    //@{
     Population( Model& model, const ASN1T_MsgPopulationCreation& msg );
    ~Population();
    //@}

    //! @name Accessors
    //@{
          unsigned long                               GetID            () const;
    const ModelsContainer< PopulationConcentration >& GetConcentrations() const;
    const ModelsContainer< PopulationFlow          >& GetFlows         () const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationUpdate& msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationCreation&    msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationUpdate&      msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationDestruction& msg );
    void Update        ( const ASN1T_MsgPopulationFluxCreation&    msg );
    void Update        ( const ASN1T_MsgPopulationFluxUpdate&      msg );
    void Update        ( const ASN1T_MsgPopulationFluxDestruction& msg );
    void SendCreation  ( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Population( const Population& );            //!< Copy constructor
    Population& operator=( const Population& ); //!< Assignement operator
    //@}

private:
          Model&        model_;
    const unsigned long nID_;
    const unsigned long nType_;
    const std::string   strName_;
    const Side&         side_;
    
    unsigned int        nDominationState_;

    ModelsContainer< PopulationConcentration > concentrations_;
    ModelsContainer< PopulationFlow          > flows_;
};

}

#include "Population.inl"

#endif // __Population_h_
