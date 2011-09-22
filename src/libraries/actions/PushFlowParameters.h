// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPushFlowParameters_h_
#define __ActionParameterPushFlowParameters_h_

#include "Parameter.h"

namespace sword {
    class PushFlowParameters;
    class PointList;
}

namespace kernel {
    class DotationType;
    class Automat_ABC;
    class EntityResolver_ABC;
    class EquipmentType;
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  MedicalPriorities
    @brief  MedicalPriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class PushFlowParameters : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PushFlowParameters( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
             PushFlowParameters( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver, xml::xistream& xis );
    virtual ~PushFlowParameters();
    //@}

    //! @name Operations
    //@{
    void AddResource   ( const kernel::DotationType& type, unsigned long quantity, const kernel::Automat_ABC& recipient );
    void AddTransporter( const kernel::EquipmentType& type, unsigned long quantity );
    void SetPath       ( const T_PointVector& path, const kernel::Automat_ABC& recipient );
    void SetWayBackPath( const T_PointVector& path );

    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::DotationType*, unsigned long > T_Resources;
    struct Recipient
    {
        T_Resources resources_;
        T_PointVector path_;
    };
    typedef std::map< const kernel::Automat_ABC*, Recipient >    T_Recipients;
    typedef std::map< const kernel::EquipmentType*, unsigned long > T_Equipments;
    //@}

    //! @name Copy/Assignment
    //@{
    PushFlowParameters( const PushFlowParameters& );            //!< Copy constructor
    PushFlowParameters& operator=( const PushFlowParameters& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    void Serialize( const T_PointVector& path, const std::string& tag, xml::xostream& xos ) const;
    void CommitTo( const T_PointVector& path, sword::PointList& msgPath ) const;

    void ReadRecipient  ( xml::xistream& xis, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver );
    void ReadResource   ( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, T_Resources& resources );
    void ReadTransporter( xml::xistream& xis, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver );
    void ReadPoint      ( xml::xistream& xis, T_PointVector& points );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_Recipients recipients_;
    T_Equipments transporters_;
    T_PointVector wayBackPath_;
    //@}
};

    }
}

#endif // __ActionParameterPushFlowParameters_h_
