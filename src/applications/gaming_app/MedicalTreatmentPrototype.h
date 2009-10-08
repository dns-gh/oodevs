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

struct ASN1T_ObjectAttributeMedicalTreatment;
struct ASN1T_MagicActionCreateObject;

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
             MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver, ASN1T_MagicActionCreateObject& msg );
    virtual ~MedicalTreatmentPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalTreatmentPrototype( const MedicalTreatmentPrototype& );            //!< Copy constructor
    MedicalTreatmentPrototype& operator=( const MedicalTreatmentPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;    
    //@}
};

#endif // __MedicalTreatmentPrototype_h_
