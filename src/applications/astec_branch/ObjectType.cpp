// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectType.h"


// -----------------------------------------------------------------------------
// Name: ObjectType constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::ObjectType( unsigned long id, const std::string& name, IDManager& manager )
    : manager_( manager )
    , id_( id )
    , name_( name )
    , texture_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectType destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType::~ObjectType()
{
    glDeleteTextures( 1, &texture_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GetName
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const std::string& ObjectType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: ObjectType::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ObjectType::Draw( const geometry::Point2f& where, const GlTools_ABC& /*tools*/ ) const
{
    glPushMatrix();
    glPushAttrib( GL_TEXTURE_BIT );
        if( ! texture_ )
            const_cast< ObjectType* >( this )->GenerateTexture();
        glTranslatef( where.X(), where.Y(), 0.f );
        glBindTexture( GL_TEXTURE_2D, texture_ );
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 1.f );
            glVertex2f( -450.f, -450.f );
            glTexCoord2f( 1.f, 1.f );
            glVertex2f( 450.f, -450.f );
            glTexCoord2f( 1.f, 0.f );
            glVertex2f( 450.f, 450.f );
            glTexCoord2f( 0.f, 0.f );
            glVertex2f( -450.f, 450.f );
        glEnd();
    glPopAttrib();
    glPopMatrix();
}

// -----------------------------------------------------------------------------
// Name: ObjectType::TypeIcon
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
QImage ObjectType::TypeIcon()
{
    switch( id_ )
    {
        case eObjectType_ZoneProtegee           : return QImage( xpm_objectTypeZoneProtegee  );
        case eObjectType_ZonePoserHelicoptere   : return QImage( xpm_objectTypeZonePoserHelicoptere );
        case eObjectType_ZoneNbc                : return QImage( xpm_objectTypeZoneNBC );
        case eObjectType_ZoneMobiliteAmelioree  : return QImage( xpm_objectTypeZoneMobiliteAmelioree );
        case eObjectType_ZoneInterditeTir       : return QImage( xpm_objectTypeZoneInterditeTir );
        case eObjectType_ZoneInterditeMouvement : return QImage( xpm_objectTypeZoneInterditeMvt );
        case eObjectType_ZoneImplantationLrm    : return QImage( xpm_objectTypeZoneImplantationLrm  );
        case eObjectType_ZoneImplantationCobra  : return QImage( xpm_objectTypeZoneImplantationCobra );
        case eObjectType_ZoneImplantationCanon  : return QImage( xpm_objectTypeZoneImplantationCanon );
        case eObjectType_ZoneBrouillageBrod     : return QImage( xpm_objectTypeZoneBrouillardBrod  );
        case eObjectType_ZoneBrouillageBromure  : return QImage( xpm_objectTypeZoneBrouillageBromure );
        case eObjectType_TerrainLargage         : return QImage( xpm_objectTypeTerrainLargage );
        case eObjectType_SiteFranchissement     : return QImage( xpm_objectTypeSiteFranchissement );
        case eObjectType_SiteDecontamination    : return QImage( xpm_objectTypeSiteDecontamination );
        case eObjectType_Rota                   : return QImage( xpm_objectTypeRota  );
        case eObjectType_PosteTir               : return QImage( xpm_objectTypePosteTir );
        case eObjectType_PosteControle          : return QImage( xpm_objectTypePosteControle );
        case eObjectType_PontFlottantContinu    : 
        case eObjectType_PontFlottantDiscontinu : return QImage( xpm_objectTypePontFlottant );
        case eObjectType_PlotRavitaillement     : return QImage( xpm_objectTypePlotRavitaillement );
        case eObjectType_Plateforme             : return QImage( xpm_objectTypePlateforme );
        case eObjectType_Piste                  : return QImage( xpm_objectTypePiste );
        case eObjectType_NuageNbc               : return QImage( xpm_objectTypeNuageNBC );
        case eObjectType_BouchonMines           : 
        case eObjectType_ZoneMineeLineaire      : 
        case eObjectType_ZoneMineeParDispersion : return QImage( xpm_objectTypeMine );
        case eObjectType_ItineraireLogistique   : return QImage( xpm_objectTypeItineraireLog );
        case eObjectType_ZoneImplantationMortier: return QImage( xpm_objectTypeImplantationMortier );
        case eObjectType_FosseAntiChar          : return QImage( xpm_objectTypeFosseAC );
        case eObjectType_Eboulement             : return QImage( xpm_objectTypeEboulement );
        case eObjectType_DestructionRoute       : return QImage( xpm_objectTypeDestructionRoute );
        case eObjectType_DestructionPont        : return QImage( xpm_objectTypeDestructionPont );
        case eObjectType_CampRefugies           : return QImage( xpm_objectTypeCampRefugies );
        case eObjectType_CampPrisonniers        : return QImage( xpm_objectTypeCampPrisonniers );
        case eObjectType_Barricade              : return QImage( xpm_objectTypeBarricade );
        case eObjectType_AirePoser              : return QImage( xpm_objectTypeAirePoser );
        case eObjectType_AireLogistique         : return QImage( xpm_objectTypeAireLogistique );
        case eObjectType_Abattis                : return QImage( xpm_objectTypeAbattis );
    default:
        return QImage();
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectType::GenerateTexture
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ObjectType::GenerateTexture()
{
    QImage image = TypeIcon().convertDepth( 32 );
    if( image.isNull() )
        return;

    int nTrueWidth = image.width();
    int nTrueHeight = image.height();
    int nWidth = 2;
    int nHeight = 2;
    while( nWidth < nTrueWidth )
        nWidth *= 2;
    while( nHeight < nTrueHeight )
        nHeight *= 2;

    QImage resizedImage = image.copy( 0, 0, nWidth, nHeight );

    glGenTextures( 1, & texture_ );
    glBindTexture( GL_TEXTURE_2D, texture_ );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}
