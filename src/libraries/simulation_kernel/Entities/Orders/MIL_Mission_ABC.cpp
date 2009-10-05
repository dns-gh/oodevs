// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Mission_ABC.h"
#include "MIL_MissionType_ABC.h"
#include "MIL_MissionParameterFactory.h"
#include "simulation_orders/MIL_MissionParameter_ABC.h"
#include "MIL_MissionParameterVisitor_ABC.h"

namespace
{
    void FillParameters( int firstIndex, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters_, const ASN1T_MissionParameters& parameters, const DEC_KnowledgeResolver_ABC& resolver )
    {
        for( unsigned int i = firstIndex; i < parameters.n; ++i )
            parameters_.push_back( MIL_MissionParameterFactory::Create( *(parameters.elem + i ), resolver ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver )
    : type_             ( type )
    , context_          ( true ) // $$$$ SBO 2008-12-11: Context must be present!
    , knowledgeResolver_( knowledgeResolver )
{    
    // No parameters $$$
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters )
    : type_             ( type )
    , context_          ()
    , knowledgeResolver_( knowledgeResolver )
{
    FillParameters( context_.Length(), parameters_, parameters, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const ASN1T_MissionParameters& parameters, const MT_Vector2D& refPosition )
    : type_             ( type )
    , context_          ( parameters, refPosition )
    , knowledgeResolver_( knowledgeResolver )
{
    FillParameters( context_.Length(), parameters_, parameters, knowledgeResolver );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& parent )
    : type_             ( type )
    , context_          ( parent.context_ )
    , knowledgeResolver_( knowledgeResolver )
{
    // Parameters will be filled by DIA $$$ // $$$$ LDC: TODO Fill parameters_ from DIA....
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const DEC_KnowledgeResolver_ABC& knowledgeResolver, const MIL_Mission_ABC& rhs )
    : type_             ( rhs.type_ )
    , context_          ( rhs.context_ )
    , knowledgeResolver_( knowledgeResolver )
    , parameters_       ( rhs.parameters_ )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC destructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::~MIL_Mission_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetName
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
const std::string& MIL_Mission_ABC::GetName() const
{
    return type_.GetName();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Serialize
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Serialize( ASN1T_MissionParameters& asn ) const
{
    if( type_.Copy( parameters_, asn, knowledgeResolver_, context_ ) )
        context_.Serialize( asn );
    else
        throw std::runtime_error( std::string( "Mission " ) + GetName() + " impossible to serialize parameters" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::CleanAfterSerialization
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::CleanAfterSerialization( ASN1T_MissionParameters& asn ) const
{
    context_.CleanAfterSerialization( asn );
    type_.CleanAfterSerialization( asn, context_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Accept
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Accept( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const
{
    context_.Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::UsedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::UsedByDIA()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::ReleasedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::ReleasedByDIA()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Visit
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const
{
    unsigned int parametersNumber = parameters_.size();
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        const std::string& paramName = type_.GetParameterName( i );
        const MIL_ParameterType_ABC& paramType = type_.GetParameterType( i );
        parameterVisitor.Accept( paramName, paramType, *parameters_[i] );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::SetParameter
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::SetParameter( const std::string& name, boost::shared_ptr< MIL_MissionParameter_ABC > param )
{
    unsigned int index = type_.GetParameterIndex( name );
    if( parameters_.size() <= index )
        parameters_.resize( index + 1 );
    parameters_[index] = param;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< TER_Localisation > pLocation )
{
    unsigned int index = type_.GetParameterIndex( name );
    if( parameters_.size() <= index )
        parameters_.resize( index + 1 );
    parameters_[index]->Append( pLocation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetType
// Created: NLD 2006-11-22
// -----------------------------------------------------------------------------
const MIL_MissionType_ABC& MIL_Mission_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetDirDanger
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_Mission_ABC::GetDirDanger() const
{
    return context_.GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetFuseau
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_Mission_ABC::GetFuseau() const
{
    return context_.GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AffectFuseau
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AffectFuseau( const MIL_Fuseau& fuseau )
{
    context_.AffectFuseau( fuseau );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AffectDirection
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AffectDirection( const MT_Vector2D& direction )
{
    context_.AffectDirection( direction );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetLimas
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
const T_LimaVector& MIL_Mission_ABC::GetLimas() const
{
    return context_.GetLimas();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindLima( uint nID )
{
    return context_.FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindLima( const MIL_LimaFunction& function )
{
    return context_.FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::FindNextScheduledLima
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_Mission_ABC::FindNextScheduledLima()
{
    return context_.FindNextScheduledLima();
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& MIL_Mission_ABC::GetDIAType() const
{
    return GetType().GetDIAType();
}
