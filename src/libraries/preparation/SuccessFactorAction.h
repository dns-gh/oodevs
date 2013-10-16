// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorAction_h_
#define __SuccessFactorAction_h_

#include <tools/Resolver.h>

namespace xml
{
    class xistream;
    class xostream;
}

class SuccessFactorActionParameter;
class SuccessFactorActionType;
class SuccessFactorActionTypes;

// =============================================================================
/** @class  SuccessFactorAction
    @brief  SuccessFactorAction
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorAction : public tools::Resolver< SuccessFactorActionParameter >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorAction( const SuccessFactorActionType& type );
             SuccessFactorAction( xml::xistream& xis, const SuccessFactorActionTypes& actions );
    virtual ~SuccessFactorAction();
    //@}

    //! @name Accessors
    //@{
    const SuccessFactorActionType& GetType() const;
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void AddParameter( const QString& name, const QString& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorAction( const SuccessFactorAction& );            //!< Copy constructor
    SuccessFactorAction& operator=( const SuccessFactorAction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const SuccessFactorActionType& type_;
    //@}
};

#endif // __SuccessFactorAction_h_
