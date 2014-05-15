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

#include "Resolver2.h"
#include <tools/Resolver.h>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
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
    class InfrastructureType;
    class ResourceNetworkType;
    class VolumeType;
    class MaterialCompositionType;
    class RoofShapeType;
    class LogisticSupplyClass;
    class UrbanTemplateType;

// =============================================================================
/** @class  ObjectTypes
    @brief  ObjectTypes
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectTypes
    : public tools::StringResolver< ObjectType >
    , public Resolver2< DotationType > // $$$$ AGE 2006-04-05: pas du tout des objets...
    , public tools::Resolver< WeaponSystemType, std::string >
    , public Resolver2< EquipmentType >
    , public Resolver2< NBCAgent >
    , public tools::StringResolver< FireClass >
    , public Resolver2< BreakdownType >
    , public tools::Resolver< VolumeType >
    , public tools::StringResolver< LogisticSupplyClass >
    , public tools::StringResolver< ResourceNetworkType >
    , public tools::StringResolver< MaterialCompositionType >
    , public tools::StringResolver< RoofShapeType >
    , public tools::StringResolver< InfrastructureType >
    , public tools::StringResolver< UrbanTemplateType >
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

    //! @name Accessors
    //@{
    bool GetRepairDurationInManHours() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectTypes( const ObjectTypes& );            //!< Copy constructor
    ObjectTypes& operator=( const ObjectTypes& ); //!< Assignment operator
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
    void ReadFires( xml::xistream& xis );
    void ReadFireClasses( xml::xistream& xis );
    void ReadBreakdowns( xml::xistream& xis );
    void ReadBreakdownCategory( xml::xistream& xis );
    void ReadBreakdown( xml::xistream& xis, const std::string& category );
    void ReadVolumes( xml::xistream& xis );
    void ReadVolume( xml::xistream& xis );
    void ReadResourceNetworks( xml::xistream& xis );
    void ReadResourceNetwork( xml::xistream& xis );
    void ReadUrbanTypes( xml::xistream& xis );
    void ReadMaterialCompositionType( xml::xistream& xis );
    void ReadRoofShapeType( xml::xistream& xis );
    void ReadInfrastructureType( xml::xistream& xis );
    void ReadLogisticSupplyClasses( xml::xistream& xis );
    void ReadLogisticSupplyClass( xml::xistream& xis );
    void ReadUrbanTemplateTypes( xml::xistream& xis );
    void ReadUrbanTemplateType( xml::xistream& xis );
    //@}

private:
    static unsigned int nVolumeId;
    bool repairDurationInManHours_;
};

}

#endif // __ObjectTypes_h_
