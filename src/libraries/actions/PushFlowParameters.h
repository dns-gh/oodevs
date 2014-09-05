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
#include <boost/optional.hpp>
#include "protocol/Protocol.h"

namespace sword
{
    class PushFlowParameters;
    class Pathfind;
}

namespace kernel
{
    class DotationType;
    class Automat_ABC;
    class EntityResolver_ABC;
    class EquipmentType;
    class CoordinateConverter_ABC;
    class Pathfind_ABC;
}

namespace actions
{
    namespace parameters
    {

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
             PushFlowParameters( const kernel::OrderParameter& parameter,
                                 const kernel::CoordinateConverter_ABC& converter,
                                 bool isSupply );
             PushFlowParameters( const kernel::OrderParameter& parameter,
                                 const kernel::CoordinateConverter_ABC& converter,
                                 const kernel::EntityResolver_ABC& entityResolver,
                                 const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver,
                                 const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver,
                                 const sword::PushFlowParameters& parameters );
    virtual ~PushFlowParameters();
    //@}

    //! @name Operations
    //@{
    void AddResource   ( const kernel::DotationType& type, unsigned long quantity, const kernel::Entity_ABC& recipient );
    void AddTransporter( const kernel::EquipmentType& type, unsigned long quantity );
    void SetPath       ( const kernel::Pathfind_ABC* pathfind, const kernel::Entity_ABC& recipient );
    void SetWayBackPath( const kernel::Pathfind_ABC& pathfind );

    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Map< const kernel::DotationType*, unsigned long > T_Resources;
    struct Recipient
    {
        T_Resources resources_;
        boost::optional< sword::Pathfind > path_;
    };
    typedef std::map< const kernel::Entity_ABC*, Recipient > T_Recipients;
    typedef std::map< const kernel::EquipmentType*, unsigned long > T_Equipments;
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
    bool isSupply_;
    T_Recipients recipients_;
    std::vector< const kernel::Entity_ABC* > recipientsSequence_;
    T_Equipments transporters_;
    boost::optional< sword::Pathfind > wayBackPath_;
    //@}
};

    }
}

#endif // __ActionParameterPushFlowParameters_h_
