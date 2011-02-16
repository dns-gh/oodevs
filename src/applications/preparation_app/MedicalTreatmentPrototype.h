// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentPrototype_h_
#define __MedicalTreatmentPrototype_h_

#include "clients_gui/MedicalTreatmentPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
    class ObjectType;
    // class ShapeObject;
    // class ShapeObjectLoader_ABC;
    class Team_ABC;
}
// class TeamsModel;

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
             MedicalTreatmentPrototype( QWidget* parent, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver, kernel::Object_ABC*& creation );
    virtual ~MedicalTreatmentPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
/*  JCR: TODO $$$$: to be activated
    virtual bool CanLoad();
    virtual void CommitShapeObject( const kernel::ShapeObject& importObject );
    virtual kernel::ShapeObjectLoader_ABC* LoadObjects( const std::string& filename, const kernel::CoordinateConverter_ABC& coordinatesConverter );
*/
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentPrototype( const MedicalTreatmentPrototype& );            //!< Copy constructor
    MedicalTreatmentPrototype& operator=( const MedicalTreatmentPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __MedicalTreatmentPrototype_h_
