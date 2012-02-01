// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationType.h $
// $Author: Nld $
// $Modtime: 17/03/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_DotationType.h $
//
// *****************************************************************************

#ifndef __PHY_DotationType_h_
#define __PHY_DotationType_h_

#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_DotationCategory;

// =============================================================================
// @class  PHY_DotationType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    static PHY_DotationType* munition_;
    static PHY_DotationType* carburant_;
    static PHY_DotationType* mine_;
    static PHY_DotationType* explosif_;
    static PHY_DotationType* barbele_;
    static PHY_DotationType* piece_;
    static PHY_DotationType* ration_;
    static PHY_DotationType* agentExtincteur_;
    static PHY_DotationType* energie_;
    static PHY_DotationType* funeraire_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const PHY_DotationType*     FindDotationType    ( const std::string& strName );
    static const PHY_DotationType*     FindDotationType    ( unsigned int nID );
    static const PHY_DotationCategory* FindDotationCategory( unsigned int nID );
    static const PHY_DotationCategory* FindDotationCategory( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          unsigned int GetID  () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_DotationType& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_DotationType
    {
        eMunition        = 0,
        eCarburant       = 1,
        eExplosif        = 2,
        eMine            = 3,
        eBarbele         = 4,
        ePiece           = 5,
        eRation          = 6,
        eAgentExtincteur = 7,
        eEnergie         = 8,
        eFuneraire       = 9
    };

    typedef std::map< std::string, const PHY_DotationType* > T_DotationTypeMap;
    typedef T_DotationTypeMap::const_iterator              CIT_DotationTypeMap;

    typedef std::map< std::string, const PHY_DotationCategory* > T_DotationCategoryMap;
    typedef T_DotationCategoryMap::const_iterator              CIT_DotationCategoryMap;

    typedef std::map< unsigned int, const PHY_DotationCategory* > T_DotationCategoryIDMap;
    typedef T_DotationCategoryIDMap::const_iterator             CIT_DotationCategoryIDMap;
    //@}

private:
     PHY_DotationType( const std::string& strName, E_DotationType nType );
    ~PHY_DotationType();

    //! @name Initialisation
    //@{
    const PHY_DotationCategory* InternalFindDotationCategory( unsigned int nID ) const;
    const PHY_DotationCategory* InternalFindDotationCategory( const std::string& strName ) const;

    void RegisterDotation( const std::string& strCategoryName, xml::xistream& xis );
    //@}
    
    //! @name Helpers
    //@{
    static void ReadDotation( xml::xistream& xis );
    //@}

private:
    const std::string               strName_;
    const E_DotationType            nType_;
          T_DotationCategoryMap     dotationCategories_;

private:
    static T_DotationTypeMap       dotationTypes_;
    static T_DotationCategoryIDMap dotationCategorieIDs_;
};

#endif // __PHY_DotationType_h_
