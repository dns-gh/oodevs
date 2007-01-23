// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-11-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_IDManager_h_
#define __MIL_IDManager_h_

#include "MIL.h"

#include "MIL_MOSIDManager.h"

class MIL_Config;

// =============================================================================
// Created: NLD 2005-11-04
// =============================================================================
class MIL_IDManager
{
public:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_MOSIDManager* >  T_IDManagerMap;
    typedef T_IDManagerMap::const_iterator       CIT_IDManagerMap;

    typedef std::map< std::string, MIL_MOSIDManager*, sCaseInsensitiveLess > T_ObjectIDManager;
    typedef T_ObjectIDManager::const_iterator                                CIT_ObjectIDManager;
    //@}

public:
    //! @name Operations
    //@{
    static void              Initialize         ( MIL_Config& config );
    static MIL_MOSIDManager* FindObjectIDManager( const std::string& strName );
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > static void serialize( Archive& );
    //@}


private:
    //! @name Copy/Assignement
    //@{
     MIL_IDManager();
     MIL_IDManager( const MIL_IDManager& );            //!< Copy constructor
     MIL_IDManager& operator=( const MIL_IDManager& ); //!< Assignement operator
    ~MIL_IDManager();
    //@}

public:
    static MIL_MOSIDManager units_;
    static MIL_MOSIDManager fireResultsPion_;
    static MIL_MOSIDManager fireResultsPopulation_;
    static MIL_MOSIDManager limas_;
    static MIL_MOSIDManager limits_;
    static MIL_MOSIDManager knowledgesAgent_;
    static MIL_MOSIDManager effectsWeather_;
    static MIL_MOSIDManager maintenanceComposanteStates_;
    static MIL_MOSIDManager medicalHumanStates_;
    static MIL_MOSIDManager supplyStates_;
    static MIL_MOSIDManager populations_;
    static MIL_MOSIDManager populationConcentrations_;
    static MIL_MOSIDManager populationFlows_;
    static MIL_MOSIDManager knowledgePopulations_;
    static MIL_MOSIDManager knowledgePopulationConcentrations_;
    static MIL_MOSIDManager knowledgePopulationFlows_;

private:
    static T_ObjectIDManager objectNameToType_;
    static T_IDManagerMap    objectClassIDToType_;
};


#include "MIL_IDManager.inl"

#endif // __MIL_IDManager_h_
