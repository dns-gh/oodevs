// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ColorStrategy.h"
#include "Agent.h"
#include "KnowledgeGroup.h"
#include "Team.h"
#include "Controllers.h"
#include "Object.h"

// -----------------------------------------------------------------------------
// Name: ColorStrategy constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::ColorStrategy( Controllers& controllers )
    : controllers_( controllers )
{
    InitializeSynonyms();
    InitializeColors();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy destructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::~ColorStrategy()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Agent& agent )
{
    const Team& team = agent.GetKnowledgeGroup()->GetTeam();
    const QColor color = teamColors_[ &team ].second;
    glColor3f( color.red()/255.f, color.green()/255.f, color.blue()/255.f );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Object& object )
{
    const Team& team = object.GetTeam();
    const QColor color = teamColors_[ &team ].second;
    glColor3f( color.red()/255.f, color.green()/255.f, color.blue()/255.f );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::RandomName
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
std::string ColorStrategy::RandomName() const
{
    return std::string( "toto" ) + char( 'a' + rand() % 26 ) + char( 'a' + rand() % 26 );
}

namespace 
{
    int RandomInt()
    {
        return 63 + rand() % 192;
    }
};

// -----------------------------------------------------------------------------
// Name: ColorStrategy::RandomColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
QColor ColorStrategy::RandomColor() const
{
    return QColor( RandomInt(), RandomInt(), RandomInt() );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::CreateNewColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::CreateNewColor( const std::string& name  )
{
    const std::string newColorName = RandomName();
    const QColor newColor = RandomColor();
    available_.insert( std::make_pair( newColorName, newColor ) );
    synonyms_[ name ] = newColorName;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyCreated
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyCreated( const Team& team )
{
    std::string name = team.GetName();
    std::transform( name.begin(), name.end(), name.begin(), tolower );
    const std::string realName = synonyms_[ name ];

    if( available_.empty() )
        CreateNewColor( name );

    T_Colors::iterator it = available_.find( realName );
    if( it == available_.end() )
        it =  available_.begin();

    teamColors_[ &team ] = *it;
    available_.erase( it );
}
    
// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyUpdated
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyUpdated( const Team& )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ColorStrategy::NotifyDeleted
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::NotifyDeleted( const Team& team )
{
    T_TeamColors::iterator it = teamColors_.find( &team );
    if( it != teamColors_.end() )
    {
        available_.insert( it->second );
        teamColors_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::InitializeSynonyms
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::InitializeSynonyms()
{
    // $$$$ AGE 2006-03-17: Move to a configuration file
    synonyms_[ "rouge"  ] = "rouge";
    synonyms_[ "rouges"  ] = "rouge";
    synonyms_[ "red"    ] = "rouge";
    synonyms_[ "ennemi" ] = "rouge";
    synonyms_[ "ennemis" ] = "rouge";
    synonyms_[ "enemy" ] = "rouge";
    synonyms_[ "enemies" ] = "rouge";
    synonyms_[ "eni"    ] = "rouge";
    synonyms_[ "enis"    ] = "rouge";
    synonyms_[ "glaise" ] = "rouge";
    synonyms_[ "humus" ] = "rouge";
    synonyms_[ "tourbe" ] = "rouge";
    synonyms_[ "amarante" ] = "rouge";
    synonyms_[ "amarantes" ] = "rouge";
    synonyms_[ "aniline" ] = "rouge";
    synonyms_[ "anilines" ] = "rouge";
    synonyms_[ "brique" ] = "rouge";
    synonyms_[ "briques" ] = "rouge";
    synonyms_[ "carmin" ] = "rouge";
    synonyms_[ "carmins" ] = "rouge";
    synonyms_[ "cerise" ] = "rouge";
    synonyms_[ "cerises" ] = "rouge";
    synonyms_[ "corail" ] = "rouge";
    synonyms_[ "coraux" ] = "rouge";
    synonyms_[ "corails" ] = "rouge";
    synonyms_[ "garance" ] = "rouge";
    synonyms_[ "garances" ] = "rouge";
    synonyms_[ "grenat" ] = "rouge";
    synonyms_[ "grenats" ] = "rouge";
    synonyms_[ "pourpre" ] = "rouge";
    synonyms_[ "pourpres" ] = "rouge";
    synonyms_[ "vermeil" ] = "rouge";
    synonyms_[ "vermeils" ] = "rouge";
    
    synonyms_[ "bleu"   ] = "bleu";
    synonyms_[ "bleus"   ] = "bleu";
    synonyms_[ "blue"   ] = "bleu";
    synonyms_[ "ami"    ] = "bleu";
    synonyms_[ "amis"    ] = "bleu";
    synonyms_[ "friend"    ] = "bleu";
    synonyms_[ "friends"    ] = "bleu";

    synonyms_[ "gris"   ] = "gris";
    synonyms_[ "blanc"   ] = "gris";
    synonyms_[ "blancs"   ] = "gris";
    synonyms_[ "grey"   ] = "gris";
    synonyms_[ "white"   ] = "gris";
    synonyms_[ "neutre" ] = "gris";
    synonyms_[ "neutres" ] = "gris";
    synonyms_[ "neutral" ] = "gris";
    synonyms_[ "neutrals" ] = "gris";
    synonyms_[ "argent" ] = "gris";
    synonyms_[ "argents" ] = "gris";
    synonyms_[ "silver" ] = "gris";
    synonyms_[ "silvers" ] = "gris";
    synonyms_[ "platine" ] = "gris";
    synonyms_[ "platines" ] = "gris";

    synonyms_[ "cobalt" ] = "cobalt";
    synonyms_[ "azur" ] = "cobalt";
    synonyms_[ "ardoise" ] = "cobalt";
    synonyms_[ "cobalts" ] = "cobalt";
    synonyms_[ "azurs" ] = "cobalt";
    synonyms_[ "ardoises" ] = "cobalt";

    synonyms_[ "turquoise" ] = "cyan";
    synonyms_[ "cyan" ] = "cyan";
    synonyms_[ "turquoises" ] = "cyan";
    synonyms_[ "cyans" ] = "cyan";

    synonyms_[ "orange" ] = "orange";
    synonyms_[ "oranges" ] = "orange";
    synonyms_[ "orange" ] = "orange";
    synonyms_[ "orange" ] = "orange";
    synonyms_[ "ocre" ] = "orange";
    synonyms_[ "ocres" ] = "orange";
    synonyms_[ "cuivre" ] = "orange";
    synonyms_[ "cuivres" ] = "orange";

    synonyms_[ "vert" ] = "vert";
    synonyms_[ "verts" ] = "vert";
    synonyms_[ "green" ] = "vert";
    synonyms_[ "greens" ] = "vert";
    synonyms_[ "amande" ] = "vert";
    synonyms_[ "amandes" ] = "vert";
    synonyms_[ "�meraudes" ] = "vert";
    synonyms_[ "�meraude" ] = "vert";
    synonyms_[ "jade" ] = "vert";
    synonyms_[ "jades" ] = "vert";
    synonyms_[ "m�l�ze" ] = "vert";
    synonyms_[ "m�l�zes" ] = "vert";
    synonyms_[ "olive" ] = "vert";
    synonyms_[ "olives" ] = "vert";


    synonyms_[ "marron" ] = "marron";
    synonyms_[ "marrons" ] = "marron";
    synonyms_[ "maroon" ] = "marron";
    synonyms_[ "maroons" ] = "marron";
    synonyms_[ "brown" ] = "marron";
    synonyms_[ "browns" ] = "marron";
    synonyms_[ "acajou" ] = "marron";
    synonyms_[ "acajous" ] = "marron";
    synonyms_[ "auburn" ] = "marron";
    synonyms_[ "auburns" ] = "marron";
    synonyms_[ "bruns" ] = "marron";
    synonyms_[ "brun" ] = "marron";
    synonyms_[ "caramel" ] = "marron";
    synonyms_[ "caramels" ] = "marron";
    synonyms_[ "chataigne" ] = "marron";
    synonyms_[ "chataignes" ] = "marron";
    synonyms_[ "mordor�s" ] = "marron";
    synonyms_[ "mordor�s" ] = "marron";
    synonyms_[ "noisettes" ] = "marron";
    synonyms_[ "noisettes" ] = "marron";

    synonyms_[ "noir" ] = "noir";
    synonyms_[ "noirs" ] = "noirs";
    synonyms_[ "black" ] = "noir";
    synonyms_[ "blacks" ] = "noir";
    synonyms_[ "chocolat" ] = "noir";
    synonyms_[ "chocolats" ] = "noir";
    synonyms_[ "bitume" ] = "noir";
    synonyms_[ "cachou" ] = "noir";
    synonyms_[ "bitumes" ] = "noir";
    synonyms_[ "cachous" ] = "noir";
    synonyms_[ "charbon" ] = "noir";
    synonyms_[ "carbone" ] = "noir";
    synonyms_[ "�b�ne" ] = "noir";
    synonyms_[ "ivoire" ] = "noir";
    synonyms_[ "jais" ] = "noir";
    synonyms_[ "r�glisse" ] = "noir";
    synonyms_[ "charbons" ] = "noir";
    synonyms_[ "carbones" ] = "noir";
    synonyms_[ "�b�nes" ] = "noir";
    synonyms_[ "ivoires" ] = "noir";
    synonyms_[ "r�glisses" ] = "noir";

    synonyms_[ "jaune" ] = "jaune";
    synonyms_[ "jaunes" ] = "jaune";
    synonyms_[ "gold" ] = "jaune";
    synonyms_[ "yellow" ] = "jaune";
    synonyms_[ "golds" ] = "jaune";
    synonyms_[ "yellows" ] = "jaune";
    synonyms_[ "or" ] = "jaune";
    synonyms_[ "ors" ] = "jaune";
    synonyms_[ "topaze" ] = "jaune";
    synonyms_[ "soufre" ] = "jaune";
    synonyms_[ "safran" ] = "jaune";
    synonyms_[ "mimoza" ] = "jaune";
    synonyms_[ "citron" ] = "jaune";
    synonyms_[ "chrome" ] = "jaune";
    synonyms_[ "bl�" ] = "jaune";
    synonyms_[ "ambre" ] = "jaune";
    synonyms_[ "topazes" ] = "jaune";
    synonyms_[ "soufres" ] = "jaune";
    synonyms_[ "safrans" ] = "jaune";
    synonyms_[ "mimozas" ] = "jaune";
    synonyms_[ "citrons" ] = "jaune";
    synonyms_[ "chromes" ] = "jaune";
    synonyms_[ "bl�s" ] = "jaune";
    synonyms_[ "ambres" ] = "jaune";
    synonyms_[ "sable" ] = "jaune";
    synonyms_[ "sables" ] = "jaune";

    synonyms_[ "fuchsia" ] = "magenta";
    synonyms_[ "fuchsias" ] = "magenta";
    synonyms_[ "magenta" ] = "magenta";
    synonyms_[ "magentas" ] = "magenta";
    synonyms_[ "rose" ] = "magenta";
    synonyms_[ "roses" ] = "magenta";
    synonyms_[ "mauve" ] = "magenta";
    synonyms_[ "mauves" ] = "magenta";
    
    synonyms_[ "inconnu" ] = "";
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::InitializeColors
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::InitializeColors()
{
    available_.insert( std::make_pair( "rouge",   QColor( 255, 50, 50 ) ) );
    available_.insert( std::make_pair( "bleu",    QColor( 100, 125, 255 ) ) );
    available_.insert( std::make_pair( "vert",    QColor( 0, 170, 0 ) ) );
    available_.insert( std::make_pair( "cyan",    QColor( 000, 230, 230 ) ) );
    available_.insert( std::make_pair( "magenta", QColor( 255, 000, 255 ) ) );
    available_.insert( std::make_pair( "orange",  QColor( 255, 150, 10 ) ) );
    available_.insert( std::make_pair( "marron",  QColor( 170, 90, 40 ) ) );
    available_.insert( std::make_pair( "jaune",   QColor( 255, 220, 000 ) ) );
    available_.insert( std::make_pair( "gris",    QColor( 140, 140, 140 ) ) );
    available_.insert( std::make_pair( "noir",    QColor( 80, 80, 80 ) ) );
    available_.insert( std::make_pair( "cobalt",  QColor( 50, 50, 128 ) ) );
    available_.insert( std::make_pair( "",        QColor( 255, 100, 255 ) ) );
}
