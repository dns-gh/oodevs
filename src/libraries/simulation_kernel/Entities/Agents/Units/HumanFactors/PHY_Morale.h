// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Morale.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:25 $
// $Revision: 4 $
// $Workfile: PHY_Morale.h $
//
// *****************************************************************************

#ifndef __PHY_Morale_h_
#define __PHY_Morale_h_

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum UnitAttributes_EnumUnitMorale;
}

// =============================================================================
// @class  PHY_Morale
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Morale : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Morale* > T_MoraleMap;
    typedef T_MoraleMap::const_iterator              CIT_MoraleMap;
    //@}

public:
    //! @name
    //@{
    static const PHY_Morale mauvais_;
    static const PHY_Morale moyen_;
    static const PHY_Morale bon_;
    static const PHY_Morale fanatique_;
    //@}

public:
    //! @name Manager
    //@{
    static       void        Initialize();
    static       void        Terminate ();
    static const PHY_Morale* Find      ( const std::string& strName );
    static const PHY_Morale* Find      ( sword::UnitAttributes_EnumUnitMorale nAsnID );
    static const PHY_Morale* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&        GetName  () const;
          unsigned int                GetID    () const;
          double            GetWeight() const;
          sword::UnitAttributes_EnumUnitMorale GetAsnID () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_Morale& rhs ) const;
    bool operator!=( const PHY_Morale& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_MoraleType
    {
        eFanatique = 0,
        eBon       = 1,
        eMoyen     = 2,
        eMauvais   = 3,
    };
    //@}

private:
     PHY_Morale( const std::string& strName, E_MoraleType nType, sword::UnitAttributes_EnumUnitMorale nAsnID, double rDIAWeight );
    ~PHY_Morale();

private:
    const std::string          strName_;
    const E_MoraleType         nType_;
    const sword::UnitAttributes_EnumUnitMorale nAsnID_;
    const double             rDIAWeight_;

private:
    static T_MoraleMap morales_;
};

#endif // __PHY_Morale_h_
