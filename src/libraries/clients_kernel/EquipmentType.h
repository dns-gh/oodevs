// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentType_h_
#define __EquipmentType_h_

#include <boost/noncopyable.hpp>
#include <tools/Resolver.h>

namespace xml { class xistream; };

namespace kernel
{
    class WeaponSystemType;
    class DotationCapacityType;
    class BreakdownOriginType;

// =============================================================================
/** @class  EquipmentType
    @brief  EquipmentType
*/
// Created: AGE 2006-02-21
// =============================================================================
class EquipmentType : public tools::Resolver< WeaponSystemType, std::string >
                    , private boost::noncopyable
{
public :
    //! @name Types
    //@{
    struct CarryingSupplyFunction
    {
        CarryingSupplyFunction( const std::string& stockNature,
            double stockMaxWeightCapacity, double stockMinWeightCapacity,
            double stockMaxVolumeCapacity, double stockMinVolumeCapacity )
            : stockNature_( stockNature )
            , stockMaxWeightCapacity_( stockMaxWeightCapacity )
            , stockMinWeightCapacity_( stockMinWeightCapacity )
            , stockMaxVolumeCapacity_( stockMaxVolumeCapacity )
            , stockMinVolumeCapacity_( stockMinVolumeCapacity )
        {
            // NOTHING
        }
        std::string stockNature_;
        double stockMaxWeightCapacity_;
        double stockMinWeightCapacity_;
        double stockMaxVolumeCapacity_;
        double stockMinVolumeCapacity_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             EquipmentType( xml::xistream& xis, const  tools::Resolver_ABC< WeaponSystemType, std::string >& weapons );
    virtual ~EquipmentType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    const std::string& GetProtection() const;
    unsigned long GetId() const;
    float GetWeight() const;
    tools::Iterator< const BreakdownOriginType& > CreateBreakdownsIterator() const;
    tools::Iterator< const DotationCapacityType& > CreateResourcesIterator() const;
    const CarryingSupplyFunction* GetLogSupplyFunctionCarrying() const;
    //@}

private:
    //! @name Type
    //@{
    typedef std::vector< BreakdownOriginType* >  T_Breakdowns;
    typedef std::vector< DotationCapacityType* > T_Resources;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadWeaponSystem( xml::xistream& xis, const  tools::Resolver_ABC< WeaponSystemType, std::string >& weapons );
    void ReadBreakdown( xml::xistream& xis );
    void ReadResourceCategory( xml::xistream& xis );
    void ReadResource( xml::xistream& xis );
    void ReadLogSupplyFunction( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string protection_;
    unsigned long id_;
    T_Breakdowns breakdowns_;
    T_Resources resources_;
    std::auto_ptr< CarryingSupplyFunction > carryingSupplyFunction_;
    float weight_;
    //@}
};

}

#endif // __EquipmentType_h_
