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

namespace sword {
    class PullFlowParameters;
}

namespace kernel {
    class DotationType;
    class Automat_ABC;
    class Formation_ABC;
    class EntityResolver_ABC;
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
    explicit PullFlowParameters( const kernel::OrderParameter& parameter );
             PullFlowParameters( const kernel::OrderParameter& parameter, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, xml::xistream& xis );
    virtual ~PullFlowParameters();
    //@}

    //! @name Operations
    //@{
    void SetSupplier( const kernel::Automat_ABC& supplierAutomat );
    void SetSupplier( const kernel::Formation_ABC& supplierFormation );
    void AddResource( const kernel::DotationType& type, unsigned long quantity );

    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Types
    //@{
    //typedef std::vector< E_HumanWound > T_Priorities;
    typedef std::map< const kernel::DotationType*, unsigned long > T_Resources;
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

    void ReadResource ( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver );
    //@}

private:
    //! @name Member data
    //@{
    T_Resources resources_;
    const kernel::Automat_ABC* supplierAutomat_;
    const kernel::Formation_ABC* supplierFormation_;
    //@}
};

    }
}

#endif // __ActionParameterPullFlowParameters_h_
