// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-26 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "GenObject.h"
#include "Testable_Entity.h"
#include "Tools/Location.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: GenObject constructor
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
GenObject::GenObject()
    : nId_               ( 0 )
    , pLocation_         ( 0 )
    , eType_             ( ( ASN1T_EnumObjectType )-1 )
    , eObstacleSubType_  ( ( ASN1T_EnumMissionGenSousTypeObstacle )-1 )
    , eUrgency_          ( ( ASN1T_EnumMissionGenUrgence )-1 )
    , ePriority_         ( ( ASN1T_EnumMissionGenPriorite )-1 )
{

}

// -----------------------------------------------------------------------------
// Name: GenObject destructor
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
GenObject::~GenObject()
{
    if( pLocation_ )
        delete pLocation_;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetTestParam_GenObject
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
GenObject& GenObject::GetTestParam_GenObject( const Testable_Entity& target )
{
    GenObject& obj = *new GenObject();

    obj.nId_              = target.GetTestParam_ID();
    obj.pLocation_        = &Location::GetTestParam_Location( target.GetPosition(), ( ASN1T_EnumTypeLocalisation )target.GetTestParam_Enumeration( 0, 5 ) );
    obj.eType_            = ( ASN1T_EnumObjectType )target.GetTestParam_Enumeration( 0, 38 );
    obj.eObstacleSubType_ = ( ASN1T_EnumMissionGenSousTypeObstacle )target.GetTestParam_Enumeration( 0, 1 );
    obj.eUrgency_         = ( ASN1T_EnumMissionGenUrgence )target.GetTestParam_Enumeration( 0, 1 );
    obj.ePriority_        = ( ASN1T_EnumMissionGenPriorite )target.GetTestParam_Enumeration( 0, 2 );

    return obj;
}

// -----------------------------------------------------------------------------
// Name: GenObject::GetTestParam_GenObjectList
// Created: SBO 2005-08-26
// -----------------------------------------------------------------------------
T_GenObjectVector& GenObject::GetTestParam_GenObjectList( const Testable_Entity& target, uint nGenObject )
{
    T_GenObjectVector& objVect = *new T_GenObjectVector();
    for( uint i = 0; i < nGenObject; ++i )
        objVect.push_back( &GetTestParam_GenObject( target ) );
    return objVect;
}
