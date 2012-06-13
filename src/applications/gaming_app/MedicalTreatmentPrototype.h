// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentPrototype_h_
#define __MedicalTreatmentPrototype_h_

#include "clients_gui/MedicalTreatmentPrototype_ABC.h"
#include "protocol/ClientSenders.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

struct ObjectAttributeMedicalTreatment;

// =============================================================================
/** @class  MedicalTreatmentPrototype
    @brief  MedicalTreatmentPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class MedicalTreatmentPrototype : public gui::MedicalTreatmentPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver, actions::parameters::ParameterList*& attributesList );
    virtual ~MedicalTreatmentPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit( const kernel::Team_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList*& attributesList_;
    //@}
};

#endif // __MedicalTreatmentPrototype_h_
