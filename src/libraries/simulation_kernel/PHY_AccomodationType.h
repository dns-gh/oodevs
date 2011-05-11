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

#include "MT_Tools/MT_String.h"

// =============================================================================
/** @class  PHY_AccomodationType
    @brief  PHY_AccomodationType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_AccomodationType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_AccomodationType* > T_AccomodationMap;
    typedef T_AccomodationMap::const_iterator CIT_AccomodationMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static const PHY_AccomodationType* Find( const std::string& strName );
    static const T_AccomodationMap& GetAccomodations();
    //@}

    //! @name Accessors
    //@{
    float GetNominalCapacity() const;
    float GetMaxCapacity() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PHY_AccomodationType( const std::string& role, float nominalCapacity, float maxCapacity );
    virtual ~PHY_AccomodationType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadAccomodation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    static T_AccomodationMap accomodations_;
    std::string role_;
    float nominalCapacity_;
    float maxCapacity_;
    //@}
};

#endif // __PHY_AccomodationType_h_
