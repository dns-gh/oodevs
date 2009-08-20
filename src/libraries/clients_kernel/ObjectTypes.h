// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectTypes_h_
#define __ObjectTypes_h_

#include "Resolver.h"
#include "Resolver2.h"

namespace xml { class xistream; };
namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class BreakdownType;
    class ObjectType;
    class DotationType;
    class EquipmentType;
    class NBCAgent;
    class WeaponSystemType;
    class FireClass;
    class MedicalTreatmentType;

// =============================================================================
/** @class  ObjectTypes
    @brief  ObjectTypes
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectTypes
    : public StringResolver< ObjectType >
    , public Resolver2< DotationType > // $$$$ AGE 2006-04-05: pas du tout des objets...
    , public Resolver< WeaponSystemType, std::string >
    , public Resolver< EquipmentType >
    , public Resolver2< NBCAgent >
    , public Resolver2< FireClass >
    , public Resolver2< MedicalTreatmentType >
    , public Resolver< BreakdownType >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectTypes();
    explicit ObjectTypes( const tools::ExerciseConfig& config );
    virtual ~ObjectTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectTypes( const ObjectTypes& );            //!< Copy constructor
    ObjectTypes& operator=( const ObjectTypes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadObjectTypes( xml::xistream& xis );
    void ReadObjectType( xml::xistream& xis );
    void ReadDotations( xml::xistream& xis );
    void ReadDotation( xml::xistream& xis );
    void ReadWeaponSystems( xml::xistream& xis );
    void ReadWeaponSystem( xml::xistream& xis );
    void ReadEquipments( xml::xistream& xis );
    void ReadEquipment( xml::xistream& xis );
    void ReadNBC( xml::xistream& xis );
    void ReadNBCAgent( xml::xistream& xis );
    void ReadFire( xml::xistream& xis );
    void ReadFireClass( xml::xistream& xis );
    void ReadMedicalTreatment( xml::xistream& xis );
    void ReadMedicalTreatmentType( xml::xistream& xis );
    void ReadBreakdowns( xml::xistream& xis );
    void ReadBreakdownCategory( xml::xistream& xis );
    void ReadBreakdown( xml::xistream& xis );
    //@}
};

}

#endif // __ObjectTypes_h_
