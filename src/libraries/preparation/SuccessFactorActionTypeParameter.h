// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionTypeParameter_h_
#define __SuccessFactorActionTypeParameter_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SuccessFactorActionTypeParameter
    @brief  SuccessFactorActionTypeParameter
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorActionTypeParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SuccessFactorActionTypeParameter( xml::xistream& xis );
    virtual ~SuccessFactorActionTypeParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetDefault() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionTypeParameter( const SuccessFactorActionTypeParameter& );            //!< Copy constructor
    SuccessFactorActionTypeParameter& operator=( const SuccessFactorActionTypeParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const QString default_;
    //@}
};

#endif // __SuccessFactorActionTypeParameter_h_
