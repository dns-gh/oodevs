// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/perceptions/PHY_PerceptionLevel.h $
// $Author: Jvt $
// $Modtime: 16/03/05 19:05 $
// $Revision: 2 $
// $Workfile: PHY_PerceptionLevel.h $
//
// *****************************************************************************

#ifndef __PHY_PerceptionLevel_h_
#define __PHY_PerceptionLevel_h_
#include <map>

namespace sword
{
    enum UnitIdentification_Level;
}

// =============================================================================
// @class  PHY_PerceptionLevel
// Created: JVT 2004-08-03
// =============================================================================
class PHY_PerceptionLevel : private boost::noncopyable
{
public:
    //! @name Member data
    //@{
    static const PHY_PerceptionLevel identified_;
    static const PHY_PerceptionLevel recognized_;
    static const PHY_PerceptionLevel detected_;
    static const PHY_PerceptionLevel notSeen_;
    //@}

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    bool IsBestLevel() const;

    static const PHY_PerceptionLevel& FindPerceptionLevel( unsigned int nID );
    static const unsigned int ConvertFromMsgIdToSimId( sword::UnitIdentification_Level level );
    //@}

    //! @name Operators
    //@{
    void Serialize( sword::UnitIdentification_Level& level ) const;

    bool operator ==( const PHY_PerceptionLevel& rhs ) const;
    bool operator !=( const PHY_PerceptionLevel& rhs ) const;
    bool operator < ( const PHY_PerceptionLevel& rhs ) const;
    bool operator <=( const PHY_PerceptionLevel& rhs ) const;
    bool operator > ( const PHY_PerceptionLevel& rhs ) const;
    bool operator >=( const PHY_PerceptionLevel& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Level
    {
        eNotSeen    = 0,
        eDetected   = 1,
        eRecognized = 2,
        eIdentified = 3,
    };
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PHY_PerceptionLevel( const std::string& strName, E_Level nLevel, const sword::UnitIdentification_Level& nAsnID );
    virtual ~PHY_PerceptionLevel();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, const PHY_PerceptionLevel* > T_LevelMap;
    typedef T_LevelMap::const_iterator                         CIT_LevelMap;
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_Level nLevel_;
    const sword::UnitIdentification_Level nAsnID_;
    static T_LevelMap levels_;
    //@}
};

#endif // __PHY_PerceptionLevel_h_
