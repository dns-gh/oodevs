// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPullFlowParameters_h_
#define __ActionParameterPullFlowParameters_h_

#include "Parameter.h"
#include <boost/optional.hpp>
#include "protocol/Protocol.h"

namespace sword {
    class PullFlowParameters;
}

namespace kernel {
    class Automat_ABC;
    class CoordinateConverter_ABC;
    class DotationType;
    class EntityResolver_ABC;
    class EquipmentType;
    class Formation_ABC;
    class Pathfind_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  MedicalPriorities
    @brief  MedicalPriorities
*/
// Created: SBO 2007-06-26
// =============================================================================
class PullFlowParameters : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             PullFlowParameters( const kernel::OrderParameter& parameter,
                                 const kernel::CoordinateConverter_ABC& converter );
             PullFlowParameters( const kernel::OrderParameter& parameter,
                                 const kernel::CoordinateConverter_ABC& converter,
                                 const kernel::EntityResolver_ABC& entityResolver,
                                 const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver,
                                 const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver,
                                 const sword::PullFlowParameters& parameters );
    virtual ~PullFlowParameters();
    //@}

    //! @name Operations
    //@{
    void SetSupplier( const kernel::Automat_ABC& supplierAutomat );
    void SetSupplier( const kernel::Formation_ABC& supplierFormation );
    void AddResource( const kernel::DotationType& type, unsigned long quantity );
    void AddTransporter( const kernel::EquipmentType& type, unsigned long quantity );
    void SetWayOutPath( const kernel::Pathfind_ABC& pathfind );
    void SetWayBackPath( const kernel::Pathfind_ABC& pathfind );

    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::DotationType*, unsigned long > T_Resources;
    typedef std::map< const kernel::EquipmentType*, unsigned long > T_Equipments;
    //@}

    //! @name Copy/Assignment
    //@{
    PullFlowParameters( const PullFlowParameters& );            //!< Copy constructor
    PullFlowParameters& operator=( const PullFlowParameters& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    void Serialize( const T_PointVector& path, const std::string& tag, xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_Resources resources_;
    T_Equipments transporters_;
    const kernel::Automat_ABC* supplierAutomat_;
    const kernel::Formation_ABC* supplierFormation_;
    boost::optional< sword::Pathfind > wayOutPath_;
    boost::optional< sword::Pathfind > wayBackPath_;
    //@}
};

    }
}

#endif // __ActionParameterPullFlowParameters_h_
