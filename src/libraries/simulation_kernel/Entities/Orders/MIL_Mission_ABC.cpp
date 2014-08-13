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
#include "Tools/NET_AsnException.h"
#include "protocol/Protocol.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "CheckPoints/SerializationTools.h"
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type,
                                  const DEC_KnowledgeResolver_ABC& knowledgeResolver,
                                  uint32_t id,
                                  const boost::shared_ptr< MIL_Mission_ABC >& parent )
    : type_             ( type )
    , id_               ( id )
    , parentId_         ( parent ? parent->GetId() : 0 )
    , context_          ( parent ? parent->context_ : MIL_OrderContext( true ) )
    , knowledgeResolver_( knowledgeResolver )
{
    // $$$$ LMT 2010-04-19: set default to NullParameter
    // Parameters will be filled by DIA $$$
    // $$$$ LDC: TODO Fill parameters_ from DIA....
    if( parent )
        type.InitializeDefault( parameters_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_MissionType_ABC& type,
                                  const DEC_KnowledgeResolver_ABC& knowledgeResolver,
                                  uint32_t id,
                                  const sword::MissionParameters& parameters,
                                  const boost::optional< MT_Vector2D >& orientation )
    : type_             ( type )
    , id_               ( id )
    , parentId_         ( 0 )
    , context_          ( orientation ? MIL_OrderContext( parameters, *orientation, knowledgeResolver ) : MIL_OrderContext( false ) )
    , knowledgeResolver_( knowledgeResolver )
{
    FillParameters( context_.Length(), parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_Mission_ABC::MIL_Mission_ABC( const MIL_Mission_ABC& rhs,
                                  const DEC_KnowledgeResolver_ABC& knowledgeResolver,
                                  uint32_t id )
    : type_             ( rhs.type_ )
    , id_               ( id )
    , parentId_         ( 0 )
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
    const auto& parameterTypes = type_.GetParameters();
    int i = firstIndex;
    for( auto it = parameterTypes.begin(); it != parameterTypes.end(); ++it, ++i )
    {
        try
        {
            boost::shared_ptr< MIL_MissionParameter_ABC > pParameter;
            const MIL_OrderTypeParameter& parameterType = **it;
            if( parameters.elem_size() <= i )
            {
                if( !parameterType.IsOptional() )
                    throw ORDER_BADPARAM(
                        "got " << parameters.elem_size() << " parameters, an additional "
                        << parameterType.GetType().GetName() << " is expected" );
                pParameter = boost::make_shared< MIL_NullParameter >();
            }
            else
            {
                pParameter = MIL_MissionParameterFactory::Create( parameterType,
                        parameters.elem( i ), knowledgeResolver_, orientation_ );
            }
            if( !pParameter->IsOfType( parameterType.GetType().GetType() ) )
                throw ORDER_BADPARAM( "parameter[" << i << "] must be a "
                        << parameterType.GetType().GetName() );
            if( !parameterType.IsOptional() && dynamic_cast< const MIL_NullParameter* >( pParameter.get() ) )
                throw ORDER_BADPARAM( "parameter[" << i << "] cannot be null" );
            parameters_.push_back( pParameter );
        }
        catch( const NET_AsnBadParam< sword::OrderAck::ErrorCode >& )
        {
            throw;
        }
        catch( const std::exception& e )
        {
            throw ORDER_BADPARAM( "could not handle parameter[" << i << "]: "
                    << tools::GetExceptionMsg( e ));
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
        MT_LOG_ERROR_MSG( "Mission " + GetName() + " impossible to serialize parameters" );
        throw MASA_EXCEPTION_ASN( sword::OrderAck::ErrorCode, sword::OrderAck::error_invalid_parameter );
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
    for( unsigned int i = 0; i < parametersNumber; ++i )
    {
        const std::string& paramName = type_.GetParameterName( i );
        const MIL_OrderTypeParameter& paramType = type_.GetParameterType( i );
        if( parameters_[i] )
            parameterVisitor.Accept( paramName, paramType, *parameters_[i] );
    }
}

namespace
{
    void EnsureParameters( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters, unsigned int index )
    {
        for( std::size_t i = parameters.size(); i <= index; ++i )
            parameters.push_back( boost::make_shared< MIL_NullParameter >() );
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
// Name: MIL_Mission_ABC::MakeParameters
// Created: MCO 2013-03-05
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::MakeParameters( unsigned int index )
{
    if( parameters_.size() <= index || !parameters_[index] )
        EnsureParameters( parameters_, index );
    if( dynamic_cast< MIL_NullParameter* >( parameters_[index].get() ) )
        parameters_[index] = boost::make_shared< MIL_ListParameter >( std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< TER_Localisation > pLocation )
{
    unsigned int index = type_.GetParameterIndex( name );
    MakeParameters( index );
    parameters_[index]->Append( MIL_MissionParameterFactory::CreateLocation( pLocation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< DEC_Knowledge_Object > pKnowledgeObject )
{
    unsigned int index = type_.GetParameterIndex( name );
    MakeParameters( index );
    parameters_[index]->Append( MIL_MissionParameterFactory::CreateObjectKnowledge( pKnowledgeObject ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::AppendToParameter
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::AppendToParameter( const std::string& name, boost::shared_ptr< DEC_Gen_Object > pGenObject )
{
    unsigned int index = type_.GetParameterIndex( name );
    MakeParameters( index );
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
boost::shared_ptr< MT_Vector2D > MIL_Mission_ABC::GetDirDanger() const
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
// Name: MIL_Mission_ABC::FindAllLimas
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
std::vector< MIL_LimaOrder* > MIL_Mission_ABC::FindAllLimas( const MIL_LimaFunction& function )
{
    return context_.FindAllLimas( function );
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
    throw MASA_EXCEPTION( "Invalid call of this Mission class" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetPion
// Created: MGD 2010-073-09
// -----------------------------------------------------------------------------
MIL_Automate& MIL_Mission_ABC::GetAutomate() const
{
    throw MASA_EXCEPTION( "Invalid call of this Mission class" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetOwnerId
// Created: ABR 2012-02-13
// -----------------------------------------------------------------------------
unsigned int MIL_Mission_ABC::GetOwnerId() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetId
// Created: BAX 2013-08-28
// -----------------------------------------------------------------------------
uint32_t MIL_Mission_ABC::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::GetParentId
// Created: SLI 2013-11-06
// -----------------------------------------------------------------------------
uint32_t MIL_Mission_ABC::GetParentId() const
{
    return parentId_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::load
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> context_
         >> parameters_
         >> const_cast< uint32_t& >( id_ )
         >> const_cast< uint32_t& >( parentId_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Mission_ABC::save
// Created: LGY 2011-06-06
// -----------------------------------------------------------------------------
void MIL_Mission_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << context_
         << parameters_
         << id_
         << parentId_;
}
