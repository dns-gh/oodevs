// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_AccomodationType_h_
#define __PHY_AccomodationType_h_

#include <functional>

// =============================================================================
/** @class  PHY_AccomodationType
    @brief  PHY_AccomodationType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_AccomodationType : private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    ~PHY_AccomodationType();
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static const PHY_AccomodationType* Find( const std::string& strName );
    static void Walk( const std::function< void( const PHY_AccomodationType& ) >& operand );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetRole() const;
    float GetNominalCapacity() const;
    float GetMaxCapacity() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PHY_AccomodationType( const std::string& role, float nominalCapacity, float maxCapacity );
    //@}

    //! @name Helpers
    //@{
    static void ReadAccomodation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string role_;
    const float nominalCapacity_;
    const float maxCapacity_;
    //@}
};

#endif // __PHY_AccomodationType_h_
