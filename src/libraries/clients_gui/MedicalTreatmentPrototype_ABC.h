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
#include "ValuedListItem.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <geometry/Types.h>

class QSpinBox;
class QLabel;
class QLineEdit;

namespace kernel
{
    class ObjectType;
    class MedicalTreatmentType;
    class Team_ABC;
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
             MedicalTreatmentPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver );
    virtual ~MedicalTreatmentPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
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
            QHBox* hbox = new QHBox( parent );
            new QLabel( QString( name.c_str() ), hbox );
            baseline_ = new QSpinBox( 0, 500, 1, hbox );
        }

        Capacity( const Capacity& rhs )
            : name_ ( rhs.name_ )
            , baseline_ ( rhs.baseline_ )
        {
        }

        std::string name_;
        QSpinBox*   baseline_;
    };
    typedef boost::ptr_vector< Capacity >::const_iterator CIT_Capacities;
    //@}

protected:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver_;
    boost::ptr_vector< Capacity > capacities_;
    QSpinBox*    doctors_;
    QLineEdit*   referenceID_;
    //@}
};

}

#endif // __MedicalTreatmentPrototype_ABC_h_
