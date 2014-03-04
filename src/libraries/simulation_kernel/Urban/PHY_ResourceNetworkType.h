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
    //! @name Destructor
    //@{
    ~PHY_ResourceNetworkType();
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
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
    //! @name Constructors
    //@{
    PHY_ResourceNetworkType( const std::string& strName, const PHY_DotationCategory& dotationCategory );
    //@}

    //! @name Helpers
    //@{
    static void ReadResourceNetwork( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const unsigned int nId_;
    const PHY_DotationCategory& dotationCategory_;
    //@}
};

#endif // __PHY_ResourceNetworkType_h_
