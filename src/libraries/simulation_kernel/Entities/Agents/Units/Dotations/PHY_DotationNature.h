// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationNature.h $
// $Author: Nld $
// $Modtime: 17/03/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_DotationNature.h $
//
// *****************************************************************************

#ifndef __PHY_DotationNature_h_
#define __PHY_DotationNature_h_

#include "MIL.h"

namespace xml
{
    class xostream;
}

// =============================================================================
// @class  PHY_DotationNature
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationNature : private boost::noncopyable
{

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const PHY_DotationNature* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_DotationNature& rhs ) const;
    bool operator!=( const PHY_DotationNature& rhs ) const;
    //@}

private:
    //! @name Types
    typedef std::map< std::string, const PHY_DotationNature*, sCaseInsensitiveLess > T_DotationNatureMap;
    typedef T_DotationNatureMap::const_iterator                                      CIT_DotationNatureMap;
    //@}

private:
     PHY_DotationNature( const std::string& strName );
    ~PHY_DotationNature();
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadNature( xml::xistream& xis );
    //@}

private:
    const std::string strName_;
    const uint        nID_;

private:
    static T_DotationNatureMap natures_;
    static uint                nNextID_;
};

#include "PHY_DotationNature.inl"

#endif // __PHY_DotationNature_h_
