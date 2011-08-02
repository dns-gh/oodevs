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
#include "LoadableSpinBox.h"
#include "ValuedListItem.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <geometry/Types.h>

class QLabel;

namespace kernel
{
    class ObjectType;
    class MedicalTreatmentType;
    class Team_ABC;
}

namespace gui
{
    class LoadableLineEdit;

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
             MedicalTreatmentPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver );
    virtual ~MedicalTreatmentPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void SetLoader( ObjectPrototypeLoader_ABC* loader );
    virtual void Commit() = 0;
    //@}

private:
    //! @name Helpers
    //@{
    void FillCapacityTypes( QWidget* parent );
    virtual void showEvent( QShowEvent* );
    //@}

protected:
    //! @name Capacity types
    //@{
    struct Capacity
    {
        explicit Capacity( QWidget* parent, const std::string& name )
            : name_ ( name )
        {
            Q3HBox* hbox = new Q3HBox( parent );
            new QLabel( QString( name.c_str() ), hbox );
            baseline_ = new LoadableSpinBox( 0, 500, 1, hbox );
        }

        std::string name_;
        LoadableSpinBox* baseline_;
    };
    typedef boost::ptr_vector< Capacity >::const_iterator CIT_Capacities;
    //@}

protected:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver_;
    boost::ptr_vector< Capacity > capacities_;
    LoadableSpinBox*    doctors_;
    LoadableLineEdit*   referenceID_;
    //@}
};

}

#endif // __MedicalTreatmentPrototype_ABC_h_
