// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_Enumeration_h_
#define __AGR_Enumeration_h_

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  AGR_Enumeration
    @brief  AGR_Enumeration
*/
// Created: AGN 2004-04-26
// =============================================================================
class AGR_Enumeration
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AGR_Enumeration( const std::string& strName );
    virtual ~AGR_Enumeration();
    //@}

    //! @name Operations
    //@{
    void Read( xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    const T_String_Vector& GetEnumValueSet() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Enumeration( const AGR_Enumeration& );            //!< Copy constructor
    AGR_Enumeration& operator=( const AGR_Enumeration& ); //!< Assignement operator
    //@}

private:
    //! @name Helpers
    //@{
    std::string HumanName( const std::string& strValue ) const;
    void ReadValue( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::string     strName_;
    T_String_Vector valueList_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::GetName
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
inline
const std::string& AGR_Enumeration::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::GetEnumValueSet
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
inline
const T_String_Vector& AGR_Enumeration::GetEnumValueSet() const
{
    return valueList_;
}

#endif // __AGR_Enumeration_h_
