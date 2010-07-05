// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentPrototype_ABC_h_
#define __MedicalTreatmentPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "tools/Resolver_ABC.h"
#include "ValuedComboBox.h"

class QListView;

namespace kernel
{
    class MedicalTreatmentType;
}

namespace gui
{

// =============================================================================
/** @class  MedicalTreatmentPrototype_ABC
    @brief  MedicalTreatmentPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class MedicalTreatmentPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver );
    virtual ~MedicalTreatmentPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Commit() = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentPrototype_ABC( const MedicalTreatmentPrototype_ABC& );            //!< Copy constructor
    MedicalTreatmentPrototype_ABC& operator=( const MedicalTreatmentPrototype_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void FillTypes();
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver_;
    QListView*   treatmentTypes_;
    QSpinBox*    beds_;
    QSpinBox*    availableBeds_;
    QSpinBox*    doctors_;
    QSpinBox*    availableDoctors_;
    //@}
};

}

#endif // __MedicalTreatmentPrototype_ABC_h_
