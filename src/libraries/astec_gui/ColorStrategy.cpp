// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ColorStrategy.h"
#include "GlTools_ABC.h"

#include "astec_gaming/Agent_ABC.h"
#include "astec_gaming/KnowledgeGroup.h"
#include "astec_gaming/Team.h"
#include "astec_gaming/Controllers.h"
#include "astec_gaming/Object_ABC.h"
#include "astec_gaming/Population.h"
#include "astec_gaming/Lima.h"
#include "astec_gaming/Limit.h"
#include "astec_gaming/ObjectKnowledge.h"
#include "astec_gaming/PopulationKnowledge.h"

// -----------------------------------------------------------------------------
// Name: ColorStrategy constructor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
ColorStrategy::ColorStrategy( Controllers& controllers, GlTools_ABC& tools )
    : controllers_       ( controllers )
    , tools_             ( tools )
    , selectedObject_    ( 0 )
    , selectedAgent_     ( 0 )
    , selectedSuperior_  ( 0 )
    , selectedPopulation_( 0 )
    , selectedLine_      ( 0 )
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
// Name: ColorStrategy::BeforeSelection
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::BeforeSelection()
{
    selectedObject_ = 0; selectedAgent_ = 0; selectedSuperior_ = 0; 
    selectedPopulation_ = 0; selectedLine_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Agent_ABC& element )
{
    selectedAgent_    = &element;
    selectedSuperior_ = element.GetSuperior();
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Object_ABC& element )
{
    selectedObject_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Population& element )
{
    selectedPopulation_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Lima& element )
{
    selectedLine_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::Select
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::Select( const Limit& element )
{
    selectedLine_ = &element;
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::AfterSelection
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Agent_ABC& agent )
{
    const Team& team = agent.GetTeam();
    QColor color = teamColors_[ &team ].second;
    if( selectedAgent_ == &agent )
    {
        tools_.Select( true );
        color = SelectedColor( color );
    }
    else if( ( selectedAgent_    && selectedAgent_    == agent.GetSuperior() )
          || ( selectedSuperior_ && selectedSuperior_ == agent.GetSuperior() ) )
    {
        tools_.Select( false );
        color = SuperiorSelectedColor( color );
    }
    else
        tools_.Select( false );
        
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Object_ABC& object )
{
    const Team& team = object.GetTeam();
    QColor color = teamColors_[ &team ].second;
    if( selectedObject_ == &object )
        color = SelectedColor( color );
    tools_.Select( selectedObject_ == &object );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const Population& population )
{
    const Team& team = population.GetTeam();
     QColor color = teamColors_[ &team ].second;
    if( selectedPopulation_ == &population )
        color = SelectedColor( color );
    tools_.Select( selectedPopulation_ == &population );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const AgentKnowledge& k )
{
    QColor color( 255, 255, 255 );
    const Team* team = k.GetKnowledgeTeam();
    if( team )
        color = teamColors_[ team ].second;
    color = KnowledgeColor( color );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const ObjectKnowledge& k )
{
    QColor color( 255, 255, 255 );
    const Team* team = k.GetKnowledgeTeam();
    if( team )
        color = teamColors_[ team ].second;
    color = KnowledgeColor( color );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const PopulationKnowledge& k )
{
    QColor color( 255, 255, 255 );
    const Team* team = k.GetKnowledgeTeam();
    if( team )
        color = teamColors_[ team ].second;
    color = KnowledgeColor( color );
    ApplyColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectColor
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void ColorStrategy::SelectColor( const TacticalLine_ABC& line )
{
    if( & line == selectedLine_ )
        glColor3f( 1.f, 0.5f, 0.05f );
    else
        glColor3f( 1.f, 1.f, 1.f );
    tools_.Select( & line == selectedLine_ );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SelectedColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
QColor ColorStrategy::SelectedColor( const QColor& base ) const
{
    return base.light( 150 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::SuperiorSelectedColor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
QColor ColorStrategy::SuperiorSelectedColor( const QColor& base ) const
{
    return base.light( 120 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::KnowledgeColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
QColor ColorStrategy::KnowledgeColor( const QColor& base ) const
{
    return base.dark( 120 );
}

// -----------------------------------------------------------------------------
// Name: ColorStrategy::ApplyColor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void ColorStrategy::ApplyColor( const QColor& color ) const
{
    glColor3f( color.red()   / 255.f,
               color.green() / 255.f,
               color.blue()  / 255.f );
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
    synonyms_[ "émeraudes" ] = "vert";
    synonyms_[ "émeraude" ] = "vert";
    synonyms_[ "jade" ] = "vert";
    synonyms_[ "jades" ] = "vert";
    synonyms_[ "mélèze" ] = "vert";
    synonyms_[ "mélèzes" ] = "vert";
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
    synonyms_[ "mordorés" ] = "marron";
    synonyms_[ "mordorés" ] = "marron";
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
    synonyms_[ "ébène" ] = "noir";
    synonyms_[ "ivoire" ] = "noir";
    synonyms_[ "jais" ] = "noir";
    synonyms_[ "réglisse" ] = "noir";
    synonyms_[ "charbons" ] = "noir";
    synonyms_[ "carbones" ] = "noir";
    synonyms_[ "ébènes" ] = "noir";
    synonyms_[ "ivoires" ] = "noir";
    synonyms_[ "réglisses" ] = "noir";

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
    synonyms_[ "blé" ] = "jaune";
    synonyms_[ "ambre" ] = "jaune";
    synonyms_[ "topazes" ] = "jaune";
    synonyms_[ "soufres" ] = "jaune";
    synonyms_[ "safrans" ] = "jaune";
    synonyms_[ "mimozas" ] = "jaune";
    synonyms_[ "citrons" ] = "jaune";
    synonyms_[ "chromes" ] = "jaune";
    synonyms_[ "blés" ] = "jaune";
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

