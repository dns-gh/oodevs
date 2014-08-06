// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationConcentration.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentration constructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::PopulationConcentration( kernel::Options& options, const sword::CrowdConcentrationCreation& message, const kernel::CoordinateConverter_ABC& converter, float density )
    : options_            ( options )
    , position_           ( converter.ConvertToXY( message.position() ) )
    , nID_                ( message.concentration().id() )
    , density_            ( density )
    , nHealthyHumans_     ( 0 )
    , nWoundedHumans_     ( 0 )
    , nContaminatedHumans_( 0 )
    , nDeadHumans_        ( 0 )
    , radius_             ( 0 )
    , deadRadius_         ( 0 )
    , attitude_           ( ePopulationAttitude_Calme )
{
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration destructor
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
PopulationConcentration::~PopulationConcentration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetName
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
QString PopulationConcentration::GetName() const
{
    static QString concentration( "Concentration" );
    return concentration;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetId
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
unsigned long PopulationConcentration::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::DoUpdate
// Created: HME 2005-09-30
// -----------------------------------------------------------------------------
void PopulationConcentration::DoUpdate( const sword::CrowdConcentrationUpdate& message )
{
    if( message.has_attitude()  )
        attitude_ = static_cast< E_PopulationAttitude >( message.attitude() );
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( message.has_healthy()  )
        nHealthyHumans_ = message.healthy();
    if( message.has_wounded()  )
        nWoundedHumans_ = message.wounded();
    if( message.has_contaminated()  )
        nContaminatedHumans_ = message.contaminated();
    if( message.has_dead()  )
        nDeadHumans_ = message.dead();
    if( density_ > 0 )
    {
        radius_ = std::sqrt( ( ( nHealthyHumans_ + nWoundedHumans_ + nContaminatedHumans_ + nDeadHumans_ ) / density_ ) * oneOnpi );
        deadRadius_ = std::sqrt( ( nDeadHumans_ / density_ ) * oneOnpi );
    }
    const geometry::Vector2f diag( radius_, radius_ );
    boundingBox_ = geometry::Rectangle2f( position_ - diag, position_ + diag );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetHealthyHumans() const
{
    return nHealthyHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetWoundedHumans() const
{
    return nWoundedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetContaminatedHumans() const
{
    return nContaminatedHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDeadHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int PopulationConcentration::GetDeadHumans() const
{
    return nDeadHumans_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetRadius
// Created: NPT 2012-07-26
// -----------------------------------------------------------------------------
float PopulationConcentration::GetRadius() const
{
    return radius_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetDensity
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
float PopulationConcentration::GetDensity() const
{
    return density_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetAttitude
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
QString PopulationConcentration::GetAttitude() const
{
    return tools::ToString( attitude_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetHeight
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
float PopulationConcentration::GetHeight( bool ) const
{
    return 0.;
}

namespace
{
    void SelectColor( E_PopulationAttitude attitude, GLfloat alpha )
    {
        if( attitude == ePopulationAttitude_Agressive )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGRESSIVE, alpha );
        else if( attitude == ePopulationAttitude_Excitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_EXCITED, alpha );
        else if( attitude == ePopulationAttitude_Agitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGITATED, alpha );
        else // ePopulationAttitude_Calme
            glColor4f( COLOR_POPULATION_ATTITUDE_CALM, alpha );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetColor
// Created: JSR 2013-09-26
// -----------------------------------------------------------------------------
QColor PopulationConcentration::GetColor( const std::string& option, const QColor& defaultColor ) const
{
    const QString clrString = options_.GetOption( option, QString( "" ) ).To< QString >();
    if( clrString.isEmpty() )
        return defaultColor;
    return QColor( clrString );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::SelectRightPartColor
// Created: JSR 2013-09-25
// -----------------------------------------------------------------------------
void PopulationConcentration::SelectRightPartColor( GLfloat alpha ) const
{
    static const QColor defaultHealthy( QColor::fromRgbF( COLOR_LIGHT_BLUE) );
    static const QColor defaultContaminated( Qt::green );
    static const QColor defaultWounded( Qt::red );
    static const QColor defaultDead( Qt::black );
    static const QColor defaultMostlyHealthy( Qt::yellow );
    QColor c;
    if( nWoundedHumans_ == 0 && nContaminatedHumans_ == 0 && nDeadHumans_ == 0 )
        c = GetColor( "Color/Healthy", defaultHealthy );
    else if( nHealthyHumans_ == 0 && nWoundedHumans_ == 0 && nContaminatedHumans_ == 0 )
        c = GetColor( "Color/Dead", defaultDead );
    else if( nContaminatedHumans_ > nHealthyHumans_ && nContaminatedHumans_ > nWoundedHumans_ )
        c = GetColor( "Color/Contaminated", defaultContaminated );
    else if( nWoundedHumans_ > nHealthyHumans_ && nWoundedHumans_ > nContaminatedHumans_ )
        c = GetColor( "Color/Wounded", defaultWounded );
    else // nHealthyHumans_ >= nContaminatedHumans_ && nHealthyHumans_ >= nWoundedHumans_
        c = GetColor( "Color/MostlyHealthy", defaultMostlyHealthy );
    glColor4f( c.red() / 255.f, c.green() / 255.f, c.blue() / 255.f, alpha );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Draw
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void PopulationConcentration::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& , gui::GlTools_ABC& tools ) const
{
    if( tools.IsPickingMode() )
        tools.DrawDisc( position_, radius_ );
    else
    {
        tools.DrawHalfDisc( position_, -90, radius_ );
        GLfloat currentColor[ 4 ];
        glGetFloatv( GL_CURRENT_COLOR, currentColor );
        glPushAttrib( GL_CURRENT_BIT );
        SelectRightPartColor( currentColor[ 3 ] );
        tools.DrawHalfDisc( position_, 90, radius_ );
        SelectColor( attitude_, currentColor[ 3 ] );
        tools.DrawCircle( position_, radius_ );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetPosition
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
geometry::Point2f PopulationConcentration::GetPosition( bool ) const
{
    return position_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::IsIn
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
bool PopulationConcentration::IsIn( const geometry::Rectangle2f& rect ) const
{
    return ! boundingBox_.Intersect( rect ).IsEmpty();
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f PopulationConcentration::GetBoundingBox() const
{
    return boundingBox_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void PopulationConcentration::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitCircle( position_, radius_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentration::CanAggregate
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool PopulationConcentration::CanAggregate() const
{
    return false;
}
