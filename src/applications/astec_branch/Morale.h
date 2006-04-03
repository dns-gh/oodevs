// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Morale.h $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Morale.h $
//
// *****************************************************************************

#ifndef __Morale_h_
#define __Morale_h_

#include "Types.h"
#include "ASN_Types.h"
#include "MT_Tools/MT_Tools_Types.h"

// =============================================================================
// @class  Morale
// Created: JVT 2004-08-03
// =============================================================================
class Morale
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const Morale*, sCaseInsensitiveLess > T_MoraleMap;
    typedef T_MoraleMap::const_iterator                                CIT_MoraleMap;
    //@}

public:
    //! @name 
    //@{
    static const Morale mauvais_;
    static const Morale moyen_;
    static const Morale bon_;
    static const Morale fanatique_;
    //@}

public:
    //! @name Manager
    //@{
    
    static       void         Terminate  ();
    static const Morale*  Find       ( const std::string& strName );
    static const Morale*  Find       ( ASN1T_EnumUnitMoral nAsnID );
    static const T_MoraleMap& GetMorales ();
    //@}

    //! @name Accessors
    //@{
    const std::string&        GetName () const;
          ASN1T_EnumUnitMoral GetAsnID() const;
          int                 GetID   () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const Morale& rhs ) const;
    bool operator!=( const Morale& rhs ) const;
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
    static T_MoraleMap Initialize();
     Morale( const std::string& strName, E_MoraleType nType, ASN1T_EnumUnitMoral nAsnID );
    ~Morale();
    Morale( const Morale& );
    Morale& operator=( const Morale& );

private:
    const std::string         strName_;
    const E_MoraleType        nType_;
    const ASN1T_EnumUnitMoral nAsnID_;

private:
    static T_MoraleMap morales_;
};

#include "Morale.inl"

#endif // __Morale_h_
