// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Ghost.h"
#include "Dotation.h"
#include "IdManager.h"
#include "LogisticBaseStates.h"
#include "LogisticHierarchiesBase.h"
#include "LogisticLevelAttritube.h"
#include "AgentsModel.h"
#include "GhostModel.h"
#include "Model.h"
#include "LogisticHierarchiesBase.h"
#include "FormationModel.h"
#include "ProfileHierarchies_ABC.h"
#include "ProfilesModel.h"
#include "StaticModel.h"
#include "UserProfile.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/GhostPrototype.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/MergingTacticalHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/SubTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Viewport_ABC.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Ghost constructor from creation panel
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
Ghost::Ghost( kernel::Controller& controller, const Model& model, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, const GhostPrototype& prototype )
    : EntityImplementation< Ghost_ABC >( controller, idManager.GetNextId(), prototype.name_.c_str() )
    , model_             ( model )
    , converter_         ( converter )
    , ghostType_         ( prototype.ghostType_ )
    , type_              ( prototype.type_.c_str() )
    , symbol_            ( prototype.symbol_ )
    , nature_            ( prototype.nature_ )
    , level_             ( prototype.level_ )
    , converted_         ( false )
    , logisticSuperiorID_( -1 )
{
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost constructor from xml
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
Ghost::Ghost( kernel::Controller& controller, const Model& model, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis, kernel::SymbolFactory& symbolsFactory )
    : EntityImplementation< Ghost_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name", "" ).c_str() )
    , model_             ( model )
    , converter_         ( converter )
    , ghostType_         ( ENT_Tr::ConvertToGhostType( xis.attribute< std::string >( "ghost-type", ENT_Tr::ConvertFromGhostType( eGhostType_Invalid ) ) ) )
    , type_              ( xis.attribute< std::string >( "type", "" ).c_str() )
    , nature_            ( xis.attribute< std::string >( "nature", "" ) )
    , level_             ( xis.attribute< std::string >( "level", "" ) )
    , converted_         ( false )
    , logisticSuperiorID_( -1 )
{
    assert( ghostType_ != eGhostType_Invalid && !nature_.empty() && !level_.empty() );

    if( ghostType_ == eGhostType_Automat )
    {
        ReadGhostAttributes( xis );
        E_GhostType childGhostType = eGhostType_Agent;
        xis >> xml::list( "unit", *this, &Ghost::ReadChildren );
        childGhostType = eGhostType_Automat;
        xis >> xml::list( "phantom", *this, &Ghost::ReadChildren );
    }

    symbol_ = symbolsFactory.CreateSymbol( nature_ );
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost constructor from unknown type conversion
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost::Ghost( Controller& controller, const Model& model, IdManager& idManager, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis, kernel::Entity_ABC& parent, E_GhostType ghostType )
    : EntityImplementation< Ghost_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name", "" ).c_str() )
    , model_             ( model )
    , converter_         ( converter )
    , ghostType_         ( ghostType )
    , type_              ( xis.attribute< std::string >( "type", "" ).c_str() )
    , symbol_            ( "symbols/s*gpu" )
    , nature_            ( "undefined/undefined/undefined/undefined" )
    , level_             ( ( ghostType == eGhostType_Automat ) ? "ii" : "ooo" )
    , converted_         ( true )
    , logisticSuperiorID_( -1 )
{
    assert( ghostType_ != eGhostType_Invalid );

    if( ghostType == eGhostType_Automat )
    {
        E_GhostType childGhostType = eGhostType_Agent;
        xis >> xml::list( "unit", *this, &Ghost::ReadChildren );
        childGhostType = eGhostType_Automat;
        xis >> xml::list( "phantom", *this, &Ghost::ReadChildren );
    }

    if( const kernel::TacticalHierarchies* pHierarchy = parent.Retrieve< kernel::TacticalHierarchies >() )
    {
        kernel::App6Symbol::SetKarma( symbol_, pHierarchy->GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );

        std::string levelSymbol = pHierarchy->GetLevel();
        if( !levelSymbol.empty() )
        {
            level_ = MergingTacticalHierarchies::DecreaseLevel( levelSymbol );
            if( level_.find( "levels/" ) != std::string::npos )
                level_.erase( 0, 7 );
        }
    }

    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Ghost destructor
// Created: ABR 2011-10-14
// -----------------------------------------------------------------------------
Ghost::~Ghost()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Ghost::InitializeSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void Ghost::InitializeSymbol() const
{
    kernel::App6Symbol::SetKarma( symbol_, Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Ghost::Draw
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbol_, where, -1.f );
        tools.DrawApp6Symbol( GetLevelSymbol(), where, -1.f );
    }
}

// -----------------------------------------------------------------------------
// Name: Ghost::CreateDictionary
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Ghost& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Phantom type" ), ENT_Tr::ConvertFromGhostType( ghostType_, ENT_Tr::eToTr ) );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Name" ), name_, *this, &Ghost::Rename );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Ghost", "Info/Type" ), type_ );
}

// -----------------------------------------------------------------------------
// Name: Ghost::DisplayInTooltip
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    QString id = QString( "[%L1]" ).arg( GetId() );
    displayer.Item( "" ).Start( Styles::bold )
        .Add( ( Ghost_ABC* ) this )
        .AddToDisplay( id );
    displayer.End();
}

// -----------------------------------------------------------------------------
// Name: Ghost::SerializeAttributes
// Created: ABR 2011-10-18
// -----------------------------------------------------------------------------
void Ghost::SerializeAttributes( xml::xostream& xos ) const
{
    assert( ghostType_ != eGhostType_Invalid );
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "ghost-type", ENT_Tr::ConvertFromGhostType( ghostType_, ENT_Tr::eToSim ) )
        << xml::attribute( "type", type_.ascii() )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "nature", nature_ )
        << xml::attribute( "level", level_ );
    for( CIT_Children it = children_.begin(); it != children_.end(); ++it )
        xos << xml::start( "phantom" )
                << xml::attribute( "name", it->first )
                << xml::attribute( "position", converter_.ConvertToMgrs( it->second ) )
            << xml::end; //! phantom
}

// -----------------------------------------------------------------------------
// Name: Ghost::ReadChildren
// Created: ABR 2012-07-03
// -----------------------------------------------------------------------------
void Ghost::ReadChildren( xml::xistream& xis )
{
    if( xis.has_attribute( "name" ) && xis.has_attribute( "position" ) )
        children_.push_back( T_Child( xis.attribute< std::string >( "name" ), converter_.ConvertToXY( xis.attribute< std::string >( "position" ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: Ghost::ReadGhostAttributes
// Created: ABR 2012-06-25
// -----------------------------------------------------------------------------
void Ghost::ReadGhostAttributes( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "logistic" )
            >> xml::optional >> xml::start( "superior" )
                >> xml::attribute( "id", logisticSuperiorID_ )
            >> xml::end //! superior
            >> xml::list( "subordinate", *this, &Ghost::ReadGhostSubordinate );
    ReadDotations( xis, GetId() );
    xis >> xml::end; //! logistic

    xis >> xml::optional >> xml::start( "profiles" )
            >> xml::list( "profile", *this, &Ghost::ReadGhostProfiles )
        >> xml::end; //! profile
}

// -----------------------------------------------------------------------------
// Name: Ghost::ReadGhostSubordinate
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void Ghost::ReadGhostSubordinate( xml::xistream& xis )
{
    logisticSubordinatesID_.push_back( xis.attribute( "id", -1 ) );
    ReadDotations( xis, logisticSubordinatesID_.back() );
}

// -----------------------------------------------------------------------------
// Name: Ghost::ReadGhostProfiles
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void Ghost::ReadGhostProfiles( xml::xistream& xis )
{
    if( xis.has_attribute( "read" ) )
        profilesReadOnly_.push_back( xis.attribute< std::string >( "read" ) );
    if( xis.has_attribute( "write" ) )
        profilesWriteOnly_.push_back( xis.attribute< std::string >( "write" ) );
}


// -----------------------------------------------------------------------------
// Name: Ghost::ReadDotations
// Created: ABR 2012-06-27
// -----------------------------------------------------------------------------
void Ghost::ReadDotations( xml::xistream& xis, int entityID )
{
    if( xis.has_child( "quotas" ) )
    {
        T_DotationsList& dotationList = dotations_[ entityID ];
        xis >> xml::start( "quotas" )
            >> xml::list( "resource", *this, &Ghost::ReadDotation, dotationList )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Ghost::ReadDotation
// Created: ABR 2012-06-27
// -----------------------------------------------------------------------------
void Ghost::ReadDotation( xml::xistream& xis, T_DotationsList& dotationList )
{
    assert( xis.has_attribute( "name" ) && xis.has_attribute( "quantity" ) );
    dotationList[ xis.attribute( "name", "" ) ] = xis.attribute( "quantity", -1 );
}

// -----------------------------------------------------------------------------
// Name: Ghost::FinalizeDotations
// Created: ABR 2012-06-27
// -----------------------------------------------------------------------------
void Ghost::FinalizeDotations( const ::StaticModel& staticModel, const Entity_ABC& entity, LogisticBaseStates& logHierarchy )
{
    if( dotations_.find( entity.GetId() ) != dotations_.end() )
    {
        T_DotationsList& dotationList = dotations_[ entity.GetId() ];
        for( CIT_DotationsList it = dotationList.begin(); it != dotationList.end(); ++it )
            if( DotationType* dotationType = staticModel.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( it->first ) )
                logHierarchy.SetDotation( *dotationType, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Ghost::Finalize
// Created: ABR 2012-06-26
// -----------------------------------------------------------------------------
void Ghost::Finalize( const ::StaticModel& staticModel )
{
    // Compute logistic links
    if( LogisticHierarchiesBase* logHierarchy = Retrieve< LogisticHierarchiesBase >() )
    {
        if( logisticSuperiorID_ != -1 )
            if( const kernel::Automat_ABC* superiorAutomat = model_.agents_.FindAutomat( logisticSuperiorID_ ) )
                logHierarchy->SetLogisticSuperior( superiorAutomat );
            else if( const kernel::Formation_ABC* superiorFormation = model_.formations_.Find( logisticSuperiorID_ ) )
                logHierarchy->SetLogisticSuperior( superiorFormation );
        FinalizeDotations( staticModel, *this, static_cast< LogisticBaseStates& >( *logHierarchy ) );
        for( std::vector< int >::const_iterator it = logisticSubordinatesID_.begin(); it != logisticSubordinatesID_.end(); ++it )
            if( kernel::Automat_ABC* subordinateAutomat = model_.agents_.FindAutomat( *it ) )
            {
                LogisticHierarchiesBase& subordinateHierarchy = subordinateAutomat->Get< LogisticHierarchiesBase >();
                subordinateHierarchy.SetLogisticSuperior( this );
                FinalizeDotations( staticModel, *subordinateAutomat, static_cast< LogisticBaseStates& >( subordinateHierarchy ) );
            }
            else
            {
                kernel::Ghost_ABC* subordinateGhost = model_.ghosts_.Find( *it );
                if( subordinateGhost )
                {
                    assert( subordinateGhost->GetGhostType() == eGhostType_Automat );
                    LogisticHierarchiesBase& subordinateHierarchy = subordinateGhost->Get< LogisticHierarchiesBase >();
                    subordinateHierarchy.SetLogisticSuperior( this );
                }
            }
    }
    // Compute profiles
    if( !profilesReadOnly_.empty() )
        for( std::vector< std::string >::const_iterator it = profilesReadOnly_.begin(); it != profilesReadOnly_.end(); ++it )
            if( UserProfile* profile = model_.profiles_.Find( *it ) )
                profile->SetReadable( *this, true );
    if( !profilesWriteOnly_.empty() )
        for( std::vector< std::string >::const_iterator it = profilesWriteOnly_.begin(); it != profilesWriteOnly_.end(); ++it )
            if( UserProfile* profile = model_.profiles_.Find( *it ) )
                profile->SetWriteable( *this, true );
}

// -----------------------------------------------------------------------------
// Name: Ghost::SerializeGhostAttributes
// Created: ABR 2012-06-25
// -----------------------------------------------------------------------------
void Ghost::SerializeGhostAttributes( xml::xostream& xos ) const
{
    if( const LogisticHierarchiesBase* logHierarchy = Retrieve< LogisticHierarchiesBase >() )
    {
        xos << xml::start( "logistic" );

        if( const kernel::Entity_ABC* superior = logHierarchy->GetSuperior() )
            if( superior->GetTypeName() != kernel::Ghost_ABC::typeName_ )
            {
                xos << xml::start( "superior" )
                    << xml::attribute( "id", superior->GetId() );
                xos << xml::end; //! superior
            }

        tools::Iterator< const kernel::Entity_ABC& > children = logHierarchy->CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const kernel::Entity_ABC& entity = children.NextElement();
            xos << xml::start( "subordinate" )
                << xml::attribute( "id", entity.GetId() );
            const LogisticBaseStates* subordinateLogHierarchy = dynamic_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() );
            if( subordinateLogHierarchy )
                subordinateLogHierarchy->SerializeQuotas( xos );
            xos << xml::end; //! subordinate
        }
        static_cast< const LogisticBaseStates* >( logHierarchy )->SerializeQuotas( xos );
        xos << xml::end; //! logistic
    }

    if( const ProfileHierarchies_ABC* ghostProfileHierarchy = Retrieve< ProfileHierarchies_ABC >() )
    {
        xos << xml::start( "profiles" );
        std::vector< std::string > readingProfiles = model_.profiles_.GetProfilesWhoCanRead( *this );
        for( std::vector< std::string >::const_iterator it = readingProfiles.begin(); it != readingProfiles.end(); ++it )
            xos << xml::start( "profile" )
                    << xml::attribute( "read", *it )
                << xml::end; //! profile

        std::vector< std::string > writingProfiles = model_.profiles_.GetProfilesWhoCanWrite( *this );
        for( std::vector< std::string >::const_iterator it = writingProfiles.begin(); it != writingProfiles.end(); ++it )
            xos << xml::start( "profile" )
                    << xml::attribute( "write", *it )
                << xml::end; //! profile
        xos << xml::end; //! profiles
    }
}

// -----------------------------------------------------------------------------
// Name: Ghost::Rename
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void Ghost::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetType
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
const QString& Ghost::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetGhostType
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
E_GhostType Ghost::GetGhostType() const
{
    return ghostType_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetLevel
// Created: JSR 2012-07-12
// -----------------------------------------------------------------------------
const std::string& Ghost::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string& Ghost::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetSymbol
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
const std::string& Ghost::GetNature() const
{
    return nature_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetLevelSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
const std::string Ghost::GetLevelSymbol() const
{
    return "levels/" + level_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::IsConverted
// Created: ABR 2011-10-24
// -----------------------------------------------------------------------------
bool Ghost::IsConverted() const
{
    return converted_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetChildren
// Created: ABR 2012-07-03
// -----------------------------------------------------------------------------
const Ghost_ABC::T_Children& Ghost::GetChildren() const
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: Ghost::UpdateSymbol
// Created: ABR 2011-11-04
// -----------------------------------------------------------------------------
void Ghost::UpdateSymbol( const std::string& level, const std::string& nature, const std::string& symbol )
{
    level_ = level;
    nature_ = nature;
    symbol_ = symbol;
}

// -----------------------------------------------------------------------------
// Name: Ghost::GetLogisticLevel
// Created: ABR 2012-06-25
// -----------------------------------------------------------------------------
const LogisticLevel& Ghost::GetLogisticLevel() const
{
    const LogisticLevelAttritube* logAttribut = Retrieve< LogisticLevelAttritube >();
    return logAttribut ? logAttribut->GetLogisticLevel() : LogisticLevel::none_;
}
