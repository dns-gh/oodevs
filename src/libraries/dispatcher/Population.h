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

#include "game_asn/Asn.h"
#include "ModelsContainer.h"
#include "Entity_ABC.h"
#include "DecisionalState.h"

namespace dispatcher
{
class Model;
class Side;
class PopulationConcentration;
class PopulationFlow;
class PopulationOrder;

// =============================================================================
/** @class  Population
    @brief  Population
*/
// Created: NLD 2006-09-19
// =============================================================================
class Population : public Entity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Population( Model& model, const ASN1T_MsgPopulationCreation& msg );
    virtual ~Population();
    //@}

    //! @name Accessors
    //@{
          unsigned long                               GetID            () const;
    const ModelsContainer< PopulationConcentration >& GetConcentrations() const;
    const ModelsContainer< PopulationFlow          >& GetFlows         () const;
    //@}

    //! @name Operations
    //@{
    void Update        ( const ASN1T_MsgPopulationCreation&                 msg );
    void Update        ( const ASN1T_MsgPopulationUpdate&                   msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationCreation&    msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationUpdate&      msg );
    void Update        ( const ASN1T_MsgPopulationConcentrationDestruction& msg );
    void Update        ( const ASN1T_MsgPopulationFlowCreation&             msg );
    void Update        ( const ASN1T_MsgPopulationFlowUpdate&               msg );
    void Update        ( const ASN1T_MsgPopulationFlowDestruction&          msg );
    void Update        ( const ASN1T_MsgPopulationOrder&                    msg );
    void Update        ( const ASN1T_MsgDecisionalState&                    msg );

    void SendCreation  ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor );
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
          Side&         side_;
    
    unsigned int        nDominationState_;
    PopulationOrder*    pOrder_;

    ModelsContainer< PopulationConcentration > concentrations_;
    ModelsContainer< PopulationFlow          > flows_;

    DecisionalState decisionalInfos_;
};

}

#include "Population.inl"

#endif // __Population_h_
