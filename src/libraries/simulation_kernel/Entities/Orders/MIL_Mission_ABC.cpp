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
#include "MIL_ListParameter.h"
#include "MIL_MissionType_ABC.h"
#include "MIL_MissionParameter_ABC.h"
#include "MIL_MissionParameterFactory.h"
#include "MIL_MissionParameterVisitor_ABC.h"
#include "MIL_NullParameter.h"
#include "MIL_OrderTypeParameter.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_AsnException.h"
#include "protocol/Protocol.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Checkpoints/SerializationTools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Mission_ABC )

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
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const sword::MissionParameters& parameters )
    : type_             ( type )
    , knowledgeResolver_( knowledgeResolver )
{
    FillParameters( context_.Length(), parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const sword::MissionParameters& parameters, const MT_Vector2D& refPosition )
    : type_             ( type )
    , context_          ( parameters, refPosition )
    , knowledgeResolver_( knowledgeResolver )
{
    FillParameters( context_.Length(), parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const boost::shared_ptr< MIL_Mission_ABC > parent )
    : type_             ( type )
    , context_          ( parent->context_ )
    , knowledgeResolver_( knowledgeResolver )
{
    type.InitializeDefault( parameters_ ); // $$$$ LMT 2010-04-19: set default to NullParameter
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
    // NOTHING
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
// Created: NLD 2011-11-09
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::FillParameters( int firstIndex, const sword::MissionParameters& parameters )
{
    const MIL_OrderType_ABC::T_MissionParameterVector& parameterTypes = type_.GetParameters();
    int i = firstIndex;
    for( MIL_OrderType_ABC::CIT_MissionParameterVector it = parameterTypes.begin(); it != parameterTypes.end(); ++it, ++i )
    {
        const MIL_OrderTypeParameter& parameterType = **it;
        if( parameters.elem_size() <= i )
            throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
        try
        {
            boost::shared_ptr< MIL_MissionParameter_ABC > pParameter = MIL_MissionParameterFactory::Create( parameterType, parameters.elem( i ), knowledgeResolver_ );
            if( !pParameter->IsOfType( parameterType.GetType().GetType() ) )
                throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
            if( !parameterType.IsOptional() && dynamic_cast< const MIL_NullParameter* >( pParameter.get() ) )
                throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
            parameters_.push_back( pParameter );
        }
        catch( std::exception& )
        {
            throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Serialize
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Serialize( sword::MissionParameters& asn ) const
{
    if( type_.Copy( parameters_, asn, context_ ) )
        context_.Serialize( asn );
    else
    {
        MT_LOG_ERROR_MSG( std::string( "Mission " ) + GetName() + " impossible to serialize parameters" );
        throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_parameter );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::UsedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::UsedByDIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::ReleasedByDIA
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::ReleasedByDIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::Visit
// Created: LDC 2009-04-30
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const
{
    unsigned int parametersNumber = static_cast< unsigned >( parameters_.size() );
    for (unsigned int i = 0; i < parametersNumber; ++i )
    {
        const std::string& paramName = type_.GetParameterName( i );
        const MIL_OrderTypeParameter& paramType = type_.GetParameterType( i );
        if( parameters_[i] )
            parameterVisitor.Accept( paramName, paramType, *parameters_[i] );
    }
}

namespace
{
    void EnsureParameters( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters_, unsigned int index )
    {
        if( parameters_.size() <= index )
        {
            std::size_t currentSize = parameters_.size();
            parameters_.resize( index + 1 );
            for( std::size_t i = currentSize; i < index; ++i )
            {
                boost::shared_ptr< MIL_MissionParameter_ABC > dummy ( new MIL_NullParameter() );
                parameters_[i] = dummy;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::SetParameter
// Created: LDC 2009-07-08
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::SetParameter( const std::string& name, boost::shared_ptr< MIL_MissionParameter_ABC > param )
{
    unsigned int index = type_.GetParameterIndex( name );
    EnsureParameters( parameters_, index );
    parameters_[index] = param;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< TER_Localisation > pLocation )
{
    unsigned int index = type_.GetParameterIndex( name );
    if( parameters_.size() <= index || !parameters_[index] )
        EnsureParameters( parameters_, index );
    if( dynamic_cast< MIL_NullParameter* >( parameters_[index].get() ) != 0 )
    {
        boost::shared_ptr< MIL_MissionParameter_ABC > param( new MIL_ListParameter( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >() ) );
        parameters_[index] = param;
    }
    parameters_[index]->Append( MIL_MissionParameterFactory::CreateLocation( pLocation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject )
{
    unsigned int index = type_.GetParameterIndex( name );
    if( parameters_.size() <= index || !parameters_[index] )
        EnsureParameters( parameters_, index );
    if( dynamic_cast< MIL_NullParameter* >( parameters_[index].get() ) != 0 )
    {
        boost::shared_ptr< MIL_MissionParameter_ABC > param( new MIL_ListParameter( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >() ) );
        parameters_[index] = param;
    }
    parameters_[index]->Append( MIL_MissionParameterFactory::CreateObjectKnowledge( pKnowledgeObject ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< DEC_Gen_Object > pGenObject )
{
    unsigned int index = type_.GetParameterIndex( name );
    if( parameters_.size() <= index || !parameters_[index] )
        EnsureParameters( parameters_, index );
    if( dynamic_cast< MIL_NullParameter* >( parameters_[index].get() ) != 0 )
    {
        boost::shared_ptr< MIL_MissionParameter_ABC > param( new MIL_ListParameter( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >() ) );
        parameters_[index] = param;
    }
    parameters_[index]->Append(  MIL_MissionParameterFactory::CreateGenObject( pGenObject ) );
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
MIL_LimaOrder* MIL_Mission_ABC::FindLima( unsigned int nID )
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

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetPion
// Created: MGD 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_Mission_ABC::GetPion() const
{
    throw std::runtime_error( "Invalid call of this Mission class" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetPion
// Created: MGD 2010-073-09
// -----------------------------------------------------------------------------
MIL_Automate& MIL_Mission_ABC::GetAutomate() const
{
    throw std::runtime_error( "Invalid call of this Mission class" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::load
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> context_
         >> parameters_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::save
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << context_
         << parameters_;
}
