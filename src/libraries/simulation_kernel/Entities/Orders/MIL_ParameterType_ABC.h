// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ParameterType_ABC_h_
#define __MIL_ParameterType_ABC_h_

#include "MT_Tools/MT_String.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class MissionParameter;
}

class MIL_OrderType_ABC;
class DEC_KnowledgeResolver_ABC;
class MIL_MissionParameter_ABC;

// =============================================================================
/** @class  MIL_ParameterType_ABC
    @brief  MIL Parameter type definition
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_ParameterType_ABC : private boost::noncopyable
{
public:
    enum E_Type
    {
        eBool        = 0,
        eEnumeration = 1,
        ePoint       = 2,
        ePolygon     = 3,
        eLocation    = 4,
        ePath        = 5,
        eDirection   = 6,
        eNatureAtlas = 7,
        eAutomat     = 8,
        eAgent       = 9,
        eAgentKnowledge  = 10,
        eObjectKnowledge = 11,
        eCrowdKnowledge  = 12,
        eResourceType    = 13,
        eEquipmentType   = 14,
        eDateTime        = 15,
        eInteger         = 16,
        eNumeric         = 17,
        ePlannedWork     = 18,
        eMaintenancePriorities = 19,
        eMedicalPriorities = 20,
        eIndirectFire = 21,
        eString = 22,
        eUrbanKnowledge = 23,
        eLocationComposite = 24,
        ePhaseLine = 25,
        eResourceNetwork = 26
    };

    //! @name Factory
    //@{
    static void Initialize();
    static const MIL_ParameterType_ABC* Find ( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    E_Type GetType() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ParameterType_ABC( const std::string& strName, E_Type type );
    virtual ~MIL_ParameterType_ABC();
    //@}

private:
    //! @name Initialization
    //@{
    static void RegisterParameterType( const std::string& name, E_Type type );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_ParameterType_ABC*, sCaseInsensitiveLess > T_ParameterMap;
    typedef T_ParameterMap::const_iterator                                            CIT_ParameterMap;
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    const E_Type type_;
    //@}

private:
    //! @name Member data
    //@{
    static T_ParameterMap parameters_;
    //@}
};

#endif // __MIL_ParameterType_ABC_h_
