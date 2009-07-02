// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_RoePopulation.h $
// $Author: Nld $
// $Modtime: 18/03/05 12:13 $
// $Revision: 4 $
// $Workfile: PHY_RoePopulation.h $
//
// *****************************************************************************

#ifndef __PHY_RoePopulation_h_
#define __PHY_RoePopulation_h_

// =============================================================================
// @class  PHY_RoePopulation
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoePopulation : private boost::noncopyable
{

public:
    //! @name Constants
    //@{
    static const PHY_RoePopulation none_;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_RoePopulation*, sCaseInsensitiveLess > T_RoePopulationMap;
    typedef T_RoePopulationMap::const_iterator                                      CIT_RoePopulationMap;
    //@}

public:
    //! @name Manager
    //@{
    static void                      Initialize       ();
    static void                      Terminate        ();
    static const PHY_RoePopulation*  Find             ( const std::string& strName );
    static const PHY_RoePopulation*  Find             ( uint nID );
    static const T_RoePopulationMap& GetRoePopulations();
    //@}

    //! @name Accessors
    //@{
    const std::string&              GetName () const;
          uint                      GetID   () const;
          ASN1T_EnumPopulationRoe   GetAsnID() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_RoePopulation& rhs ) const;
    bool operator!=( const PHY_RoePopulation& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eNone                                  = 0,
        eEmploiForceInterdit                   = 1,
        eMaintienADistanceParMoyensNonLetaux   = 2,
        eDispersionParMoyensDeDefenseActifs    = 3,
        eArmesLetalesAutorisees                = 4
    };
    //@}

private:
    PHY_RoePopulation( const std::string& strName, E_Type nType, ASN1T_EnumPopulationRoe nAsnID );
    ~PHY_RoePopulation();

private:
    const std::string             strName_;
    const E_Type                  nType_;
    const ASN1T_EnumPopulationRoe nAsnID_;

private:
    static T_RoePopulationMap roePopulations_;
};

#endif // __PHY_RoePopulation_h_
