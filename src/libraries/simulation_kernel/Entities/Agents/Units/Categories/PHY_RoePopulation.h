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

#include "protocol/Protocol.h"
#include "MT_Tools/MT_String.h"

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
    typedef std::map< std::string, const PHY_RoePopulation* > T_RoePopulationMap;
    typedef T_RoePopulationMap::const_iterator              CIT_RoePopulationMap;
    //@}

public:
    //! @name Manager
    //@{
    static void                      Initialize       ();
    static void                      Terminate        ();
    static const PHY_RoePopulation*  Find             ( const std::string& strName );
    static const PHY_RoePopulation*  Find             ( unsigned int nID );
    static const T_RoePopulationMap& GetRoePopulations();
    //@}

    //! @name Accessors
    //@{
    const std::string&                                      GetName () const;
          unsigned int                                      GetID   () const;
          sword::UnitAttributes::CrowdRoe  GetAsnID() const;
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
        eNone                                = 0,
        eEmploiForceInterdit                 = 1,
        eMaintienADistanceParMoyensNonLetaux = 2,
        eArmesLetalesAutorisees              = 3
    };
    //@}

private:
             PHY_RoePopulation( const std::string& strName, E_Type nType, sword::UnitAttributes::CrowdRoe nAsnID );
    virtual ~PHY_RoePopulation();

private:
    const std::string strName_;
    const E_Type nType_;
    const sword::UnitAttributes::CrowdRoe nAsnID_;

private:
    static T_RoePopulationMap roePopulations_;
};

#endif // __PHY_RoePopulation_h_
