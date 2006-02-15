// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
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

#include "astec_pch.h"
#include "Object_ABC.h"

#include "Net_Def.h"
#include "App.h"
#include "World.h"
#include "Tools.h"
#include "FireResult.h"
#include "Controller.h"
#include "xeumeuleu/xml.h"

using namespace xml;

Object_ABC::T_ObjectIDs Object_ABC::objectIds_;
Object_ABC::T_Managers  Object_ABC::managers_;

// -----------------------------------------------------------------------------
// Name: Object_ABC::Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC( const ASN1T_MsgObjectCreation& message, Controller& controller )
    : controller_( controller )
    , nId_                           ( message.oid )
    , strName_                       ( message.nom )
    , nType_                         ( message.type )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( message.localisation.type )
//    , strTypeDotationConstruction_   ()
//    , strTypeDotationValorization_   ()
//    , nNbrDotationConstruction_      ( 0 )
//    , nNbrDotationValorization_      ( 0 )
{
    InterfaceContainer< Extension_ABC >::Register( *this );

    for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }

    if( ! pointVector_.empty() )
        center_ /= pointVector_.size();

//    if( message.m.type_dotation_constructionPresent )
//    {
//        strTypeDotationConstruction_ = App::GetApp().GetResourceName( message.type_dotation_construction );
//        nNbrDotationConstruction_    = 0;
//    }
    
//    if( message.m.type_dotation_valorisationPresent )
//    {
//        strTypeDotationValorization_ = App::GetApp().GetResourceName( message.type_dotation_valorisation );
//        nNbrDotationValorization_    = 0;
//    }
}



// -----------------------------------------------------------------------------
// Name: Object_ABC::~Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::~Object_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long Object_ABC::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
const std::string& Object_ABC::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Update
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_ABC::Update( const ASN1T_MsgObjectUpdate& message )
{
    bPrepared_ = message.en_preparation;

//    if( message.m.nb_dotation_constructionPresent )
//        nNbrDotationConstruction_ = message.nb_dotation_construction;
//    if( message.m.nb_dotation_valorisationPresent )
//        nNbrDotationValorization_ = message.nb_dotation_valorisation;

    if( message.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = message.pourcentage_construction;
    if( message.m.pourcentage_valorisationPresent )
        rValorizationPercentage_ = message.pourcentage_valorisation;
    if( message.m.pourcentage_creation_contournementPresent )
        rBypassConstructionPercentage_ = message.pourcentage_creation_contournement;

    if( message.m.localisationPresent )
    {
        center_.Reset();
        pointVector_.clear();
        pointVector_.reserve( message.localisation.vecteur_point.n );
        nTypeLocalisation_ = message.localisation.type;
        for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vTmp;
            App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.localisation.vecteur_point.elem[i].data, vTmp );
            pointVector_.push_back( vTmp );
            center_ += vTmp;
        }
        if( pointVector_.size() > 1 )
            center_ /= pointVector_.size();
    }
    controller_.Update( *this );
//    App::GetApp().NotifyObjectUpdated( *this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Object_ABC::Update( const ASN1T_MsgExplosion& message )
{
//    for( uint i = 0; i < message.degats_pions.n; ++i )
//        OnReceiveMsgExplosion( message.degats_pions.elem[ i ] );

//    App::GetApp().NotifyObjectExplosion( *this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Update
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void Object_ABC::Update( const ASN1T_FireDamagesPion& message )
{

}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetIDManagerForObjectType
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
IDManager& Object_ABC::GetIDManagerForObjectType( ASN1T_EnumObjectType nType )
{
    return *managers_[ objectIds_[ nType ] ];
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetIDManagerForObjectType
// Created: HME 2005-11-08
// -----------------------------------------------------------------------------
IDManager& Object_ABC::GetIDManagerForObjectType( uint nType )
{
    return *managers_[ nType ];
}

namespace 
{
    struct ReadClass
    {
        void Read( xistream& xis )
        {
            std::string strObjectName;
            int nId;

            xis >> attribute( "nom", strObjectName )
                >> attribute( "id", nId );
            
            ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strObjectName );
            if( nType != -1 )
            {
                Object_ABC::objectIds_[ nType ] = nId;
                IDManager*& pManager = Object_ABC::managers_[ nId ];
                if( ! pManager )
                    pManager = new IDManager( nId );
            }
        };
    };
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void Object_ABC::InitializeObjectIds( xistream& xis )
{
    ReadClass readclass;
    xis >> start( "Classes" )
        >> list( "Classe", readclass, &ReadClass::Read );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::OnReceiveMsgExplosion
// Created: SBO 2005-09-07
// -----------------------------------------------------------------------------
//void Object_ABC::OnReceiveMsgExplosion( const ASN1T_FireDamagesPion& message )
//{
//    explosionResults_.push_back( new FireResult( message ) );
//    if( explosionResults_.size() > 20 )
//        explosionResults_.erase( explosionResults_.begin() );
//}
    
// -----------------------------------------------------------------------------
// Name: Object_ABC::DeleteAllFireResults
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
//void Object_ABC::DeleteAllExplosionResults()
//{
//    for( CIT_FireResults it = explosionResults_.begin(); it != explosionResults_.end(); ++it )
//        delete *it;
//    explosionResults_.clear();
//}
