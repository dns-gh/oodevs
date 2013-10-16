// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionType_h_
#define __SuccessFactorActionType_h_

#include <tools/Resolver.h>

namespace xml
{
    class xistream;
}

class SuccessFactorActionTypeParameter;

// =============================================================================
/** @class  SuccessFactorActionType
    @brief  SuccessFactorActionType
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorActionType : public tools::Resolver< SuccessFactorActionTypeParameter >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorActionType( xml::xistream& xis );
    virtual ~SuccessFactorActionType();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    std::string GetFunction() const;
    QString GetDescription() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionType( const SuccessFactorActionType& );            //!< Copy constructor
    SuccessFactorActionType& operator=( const SuccessFactorActionType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const std::string function_;
    const QString description_;
    //@}
};

#endif // __SuccessFactorActionType_h_
