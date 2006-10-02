 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentLogMedical_h_
#define __AgentLogMedical_h_

#include "AsnTypes.h"
#include "ModelRefsContainer.h"
#include "ModelsContainer.h"
#include "EquipmentAvailability.h"

namespace dispatcher
{
class Model;
class Automat;
class Publisher_ABC;

// =============================================================================
/** @class  AgentLogMedical
    @brief  AgentLogMedical
*/
// Created: NLD 2006-09-19
// =============================================================================
class AgentLogMedical
{
public:
    //! @name Constructors/Destructor
    //@{
     AgentLogMedical( Model& model, const ASN1T_MsgLogSanteEtat& asnMsg );
    ~AgentLogMedical();
    //@}

    //! @name Main
    //@{
    void Update(  const ASN1T_MsgLogSanteEtat& asnMsg );
    void Send  ( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_EnumHumanWound > T_HumanWoundVector;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentLogMedical( const AgentLogMedical& );            //!< Copy constructor
    AgentLogMedical& operator=( const AgentLogMedical& ); //!< Assignement operator
    //@}

private:
    Model&                          model_;
    bool                            bSystemEnabled_;
    T_HumanWoundVector              priorities_;
    ModelRefsContainer< Automat >   tacticalPriorities_;

    ModelsContainer< EquipmentAvailability< ASN1T_SanteDisponibiliteMoyens > > evacuationAmbulancesAvailability_;
    ModelsContainer< EquipmentAvailability< ASN1T_SanteDisponibiliteMoyens > > collectionAmbulancesAvailability_;
    ModelsContainer< EquipmentAvailability< ASN1T_SanteDisponibiliteMoyens > > doctorsAvailability_;
};

}

#endif // __AgentLogMedical_h_
