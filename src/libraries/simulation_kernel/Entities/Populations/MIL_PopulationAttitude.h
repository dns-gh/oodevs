// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_PopulationAttitude.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:05 $
// $Revision: 3 $
// $Workfile: MIL_PopulationAttitude.h $
//
// *****************************************************************************

#ifndef __MIL_PopulationAttitude_h_
#define __MIL_PopulationAttitude_h_

#include "protocol/Protocol.h"

// =============================================================================
// @class  MIL_PopulationAttitude
// Created: JVT 2004-08-03
// =============================================================================
class MIL_PopulationAttitude : private boost::noncopyable
{
private:
    //! @name Types
    //@{
    enum E_Attitude
    {
        eCalme     = 0,
        eAgitee    = 1,
        eExcitee   = 2,
        eAgressive = 3
    };
    //@}

public:
    //! @name Destructor
    //@{
    ~MIL_PopulationAttitude();
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const MIL_PopulationAttitude* Find( const std::string& strName );
    static const MIL_PopulationAttitude* Find( sword::EnumCrowdAttitude nAsnID );
    static const MIL_PopulationAttitude* Find( unsigned int nID );
    static size_t Size();
    static void Visit( const std::function< void( const MIL_PopulationAttitude& ) >& visitor );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    sword::EnumCrowdAttitude GetAsnID() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MIL_PopulationAttitude& rhs ) const;
    bool operator!=( const MIL_PopulationAttitude& rhs ) const;
    bool operator> ( const MIL_PopulationAttitude& rhs ) const;
    bool operator>=( const MIL_PopulationAttitude& rhs ) const;
    bool operator< ( const MIL_PopulationAttitude& rhs ) const;
    bool operator<=( const MIL_PopulationAttitude& rhs ) const;
    //@}

private:
    //! @name Constructor
    //@{
     MIL_PopulationAttitude( const std::string& strName, E_Attitude nAttitude, sword::EnumCrowdAttitude nAsnID );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_Attitude nAttitude_;
    const sword::EnumCrowdAttitude nAsnID_;
    //@}
};

#endif // __MIL_PopulationAttitude_h_
