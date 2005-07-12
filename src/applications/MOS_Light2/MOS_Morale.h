// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Morale.h $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: MOS_Morale.h $
//
// *****************************************************************************

#ifndef __MOS_Morale_h_
#define __MOS_Morale_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

// =============================================================================
// @class  MOS_Morale
// Created: JVT 2004-08-03
// =============================================================================
class MOS_Morale
{
    MT_COPYNOTALLOWED( MOS_Morale )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const MOS_Morale*, sCaseInsensitiveLess > T_MoraleMap;
    typedef T_MoraleMap::const_iterator                                      CIT_MoraleMap;
    //@}

public:
    //! @name 
    //@{
    static const MOS_Morale mauvais_;
    static const MOS_Morale moyen_;
    static const MOS_Morale bon_;
    static const MOS_Morale fanatique_;
    //@}

public:
    //! @name Manager
    //@{
    static       void         Initialize ();
    static       void         Terminate  ();
    static const MOS_Morale*  Find       ( const std::string& strName );
    static const MOS_Morale*  Find       ( ASN1T_EnumUnitMoral nAsnID );
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
    bool operator==( const MOS_Morale& rhs ) const;
    bool operator!=( const MOS_Morale& rhs ) const;
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
     MOS_Morale( const std::string& strName, E_MoraleType nType, ASN1T_EnumUnitMoral nAsnID );
    ~MOS_Morale();

private:
    const std::string         strName_;
    const E_MoraleType        nType_;
    const ASN1T_EnumUnitMoral nAsnID_;

private:
    static T_MoraleMap morales_;
};

#include "MOS_Morale.inl"

#endif // __MOS_Morale_h_
