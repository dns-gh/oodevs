// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationType.cpp $
// $Author: Sbo $
// $Modtime: 22/07/05 11:05 $
// $Revision: 5 $
// $Workfile: CommunicationType.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"
#include "Types.h"
#include "CommunicationType.h"
#include "MessageReport.h"

using namespace TIC;

CommunicationType::T_CommunicationTypeSet CommunicationType::comTypes_;
CommunicationType::T_TypeMap              CommunicationType::types_;
CommunicationType::T_SubTypeMap           CommunicationType::subTypes_;

const CommunicationType                   CommunicationType::notDefined( eNoType, eNoSubType );

//-----------------------------------------------------------------------------
// Name: CommunicationType::CommunicationType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
CommunicationType::CommunicationType( CommunicationType::EType eType, CommunicationType::ESubType eSubType )
    : eType_    ( eType )
    , eSubType_ ( eSubType )
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::~CommunicationType
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
CommunicationType::~CommunicationType()
{
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::Initialize
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
void CommunicationType::Initialize()
{
    // communication types
    types_.insert( std::make_pair( eOrder     , "ordre"      ) );
    types_.insert( std::make_pair( ePerception, "perception" ) );
    types_.insert( std::make_pair( eReport    , "cr"         ) );
    types_.insert( std::make_pair( eNoType    , "aucun type" ) );

    // communication subtypes
    subTypes_.insert( std::make_pair( eMission                , "mission"                 ) );
    subTypes_.insert( std::make_pair( eConduite               , "conduite"                ) );
    subTypes_.insert( std::make_pair( eUnitFriend             , "unite amie"              ) );
    subTypes_.insert( std::make_pair( eUnitEnemy              , "unite ennemie"           ) );
    subTypes_.insert( std::make_pair( eUnitNeutral            , "unite neutre"            ) );
    subTypes_.insert( std::make_pair( eUnitUnknown            , "unite inconnue"          ) );
    subTypes_.insert( std::make_pair( eUnitLost               , "unite contact perdu"     ) );
    subTypes_.insert( std::make_pair( eObject                 , "objet"                   ) );
    subTypes_.insert( std::make_pair( eObjectLost             , "objet contact perdu"     ) );
    subTypes_.insert( std::make_pair( eReportEnemy            , "cr ennemi"               ) );
    subTypes_.insert( std::make_pair( eReportObstacle         , "cr obstacle"             ) );
    subTypes_.insert( std::make_pair( eReportNbc              , "cr NBC"                  ) );
    subTypes_.insert( std::make_pair( eReportLogisticSituation, "cr situation logistique" ) );
    subTypes_.insert( std::make_pair( eReportArtillery        , "cr artillerie"           ) );
    subTypes_.insert( std::make_pair( eReportMission          , "cr mission"              ) );
    subTypes_.insert( std::make_pair( eReportScipio           , "cr scipio interne"       ) );
    subTypes_.insert( std::make_pair( eNoSubType              , "aucun sous-type"         ) );

    // communication type/subtype combinations
    comTypes_.insert( new CommunicationType( eOrder     , eMission                 ) );
    comTypes_.insert( new CommunicationType( eOrder     , eConduite                ) );
    comTypes_.insert( new CommunicationType( ePerception, eUnitFriend              ) );
    comTypes_.insert( new CommunicationType( ePerception, eUnitEnemy               ) );
    comTypes_.insert( new CommunicationType( ePerception, eUnitNeutral             ) );
    comTypes_.insert( new CommunicationType( ePerception, eUnitUnknown             ) );
    comTypes_.insert( new CommunicationType( ePerception, eUnitLost                ) );
    comTypes_.insert( new CommunicationType( ePerception, eObject                  ) );
    comTypes_.insert( new CommunicationType( ePerception, eObjectLost              ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportEnemy             ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportObstacle          ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportNbc               ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportLogisticSituation ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportArtillery         ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportMission           ) );
    comTypes_.insert( new CommunicationType( eReport    , eReportScipio            ) );
    comTypes_.insert( new CommunicationType( eReport    , eNoSubType               ) );
    comTypes_.insert( &notDefined );
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::Terminate
// Created: SBO 2005-07-12
//-----------------------------------------------------------------------------
void CommunicationType::Terminate()
{
    for( CIT_CommunicationTypeSet it = comTypes_.begin(); it != comTypes_.end(); ++it )
        if( *it != &notDefined )
            delete *it;
    comTypes_.clear();
    types_.clear();
    subTypes_.clear();
}

//-----------------------------------------------------------------------------
// Name: CommunicationType::Find
// Created: SBO 2005-07-13
//-----------------------------------------------------------------------------
const CommunicationType& CommunicationType::Find( CommunicationType::EType eType, uint nAsnId )
{
    return CommunicationType::Find( eType, MessageReport::Find( nAsnId ).GetSubType() );
}