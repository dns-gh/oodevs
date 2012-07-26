// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_ResourceNetworkType_h_
#define __PHY_ResourceNetworkType_h_

#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
/** @class  PHY_ResourceNetworkType
    @brief  PHY_ResourceNetworkType
*/
// Created: JSR 2010-09-10
// =============================================================================
class PHY_ResourceNetworkType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_ResourceNetworkType* > T_ResourceNetworkMap;
    typedef T_ResourceNetworkMap::const_iterator CIT_ResourceNetworkMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static const T_ResourceNetworkMap& GetResourceNetworks();
    static const PHY_ResourceNetworkType* Find( const std::string& strName );
    static const PHY_ResourceNetworkType* Find( unsigned int id );
    static const PHY_ResourceNetworkType* FindByDotation( const PHY_DotationCategory& dotation );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetId() const;
    const PHY_DotationCategory& GetDotationCategory() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PHY_ResourceNetworkType( const std::string& strName, const PHY_DotationCategory& dotationCategory );
    virtual ~PHY_ResourceNetworkType();
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadResourceNetwork( xml::xistream& xis );
    //@}

private:
    //! @name Static data
    //@{
    static T_ResourceNetworkMap resourceNetworks_;
    static unsigned int nNextId_;
    //@}

    //! @name Member data
    //@{
    const std::string strName_;
    unsigned int nId_;
    const PHY_DotationCategory& dotationCategory_;
    //@}
};

#endif // __PHY_ResourceNetworkType_h_
