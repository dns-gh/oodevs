// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ActiveProtection_h_
#define __PHY_ActiveProtection_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class PHY_DotationCategory;

// =============================================================================
/** @class  PHY_ActiveProtection
    @brief  PHY_ActiveProtection
*/
// Created: LDC 2010-01-07
// =============================================================================
class PHY_ActiveProtection
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_ActiveProtection( xml::xistream& xis );
    virtual ~PHY_ActiveProtection();
    //@}

    //! @name Operations
    //@{
    void UseAmmunition( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    double GetPHModifier( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    bool CounterIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;
    bool DestroyIndirectFire( const PHY_DotationCategory& category, MIL_Agent_ABC& pion ) const;

    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static PHY_ActiveProtection* Find( const std::string& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ActiveProtection( const PHY_ActiveProtection& );            //!< Copy constructor
    PHY_ActiveProtection& operator=( const PHY_ActiveProtection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadWeapon( xml::xistream& xis );
    static void Create( xml::xistream& xis );
    double GetCoefficient( const PHY_DotationCategory& category ) const;
    bool HasAmmo( MIL_Agent_ABC& pion ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< PHY_ActiveProtection > > T_ProtectionList;

    typedef std::map< const PHY_DotationCategory*, double > T_CoefficientMap;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    double coefficient_;
    double usage_;
    bool hardKill_;
    const PHY_DotationCategory* pDotation_;
    T_CoefficientMap weapons_;
    static T_ProtectionList protections_;
    //@}
};

#endif // __PHY_ActiveProtection_h_
