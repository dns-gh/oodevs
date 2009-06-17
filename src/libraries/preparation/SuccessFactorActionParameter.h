// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorActionParameter_h_
#define __SuccessFactorActionParameter_h_

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  SuccessFactorActionParameter
    @brief  SuccessFactorActionParameter
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorActionParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorActionParameter( const QString& name, const QString& value );
    explicit SuccessFactorActionParameter( xml::xistream& xis );
    virtual ~SuccessFactorActionParameter();
    //@}

    //! @name Accessors
    //@{
    QString GetName() const;
    QString GetValue() const;
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorActionParameter& operator=( const SuccessFactorActionParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    QString value_;
    //@}
};

#endif // __SuccessFactorActionParameter_h_
