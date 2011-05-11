// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:19 $
// $Revision: 3 $
// $Workfile: PHY_ConsumptionType.h $
//
// *****************************************************************************

#ifndef __PHY_ConsumptionType_h_
#define __PHY_ConsumptionType_h_

#include "MT_Tools/MT_String.h"

// =============================================================================
// @class  PHY_ConsumptionType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ConsumptionType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_ConsumptionType* > T_ConsumptionTypeMap;
    typedef T_ConsumptionTypeMap::const_iterator              CIT_ConsumptionTypeMap;
    //@}

    //! @name Modes
    //@{
    static const PHY_ConsumptionType engineStopped_;
    static const PHY_ConsumptionType engineStarted_;
    static const PHY_ConsumptionType moving_;
    static const PHY_ConsumptionType working_;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const T_ConsumptionTypeMap& GetConsumptionTypes();
    static const PHY_ConsumptionType*  FindConsumptionType( const std::string& strName );
    static const PHY_ConsumptionType*  FindConsumptionType( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          unsigned int         GetID  () const;
    //@}

    //! @name Operators
    //@{
    bool operator <( const PHY_ConsumptionType& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ConsumptionType
    {
        eEngineStopped  = 0,
        eEngineStarted  = 1,
        eMoving         = 2,
        eWorking        = 3
    };
    //@}

private:
     PHY_ConsumptionType( const std::string& strName, E_ConsumptionType nType );
    virtual ~PHY_ConsumptionType();

private:
    const std::string       strName_;
    const E_ConsumptionType nType_;

private:
    static T_ConsumptionTypeMap consumptionTypes_;
};

#endif // __PHY_ConsumptionType_h_
