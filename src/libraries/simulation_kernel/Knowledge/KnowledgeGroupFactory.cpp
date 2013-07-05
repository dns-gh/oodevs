// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
// LTO
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "KnowledgeGroupFactory.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/MIL_Army_ABC.h"
#include <boost/lexical_cast.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( KnowledgeGroupFactory )

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory constructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::KnowledgeGroupFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory destructor
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
KnowledgeGroupFactory::~KnowledgeGroupFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Create
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > KnowledgeGroupFactory::Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_KnowledgeGroup* parent )
{
    boost::shared_ptr< MIL_KnowledgeGroup > group( new MIL_KnowledgeGroup( xis, army, parent ) );
    xis >> xml::list( "knowledge-group", boost::bind( &KnowledgeGroupFactory::Create, this, _1, boost::ref( army ), group.get() ) );
    if( parent )
        parent->RegisterKnowledgeGroup( group );
    else
        army.RegisterKnowledgeGroup( group );
    Register( group->GetId(), group );
    return group;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Register
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::Register( const unsigned long& identifier, const boost::shared_ptr< MIL_KnowledgeGroup >& element )
{
    boost::shared_ptr< MIL_KnowledgeGroup >& p = elements_[ identifier ];
    if( p )
        throw MASA_EXCEPTION( "MIL_KnowledgeGroup '"
            + boost::lexical_cast< std::string >( identifier ) + "' already registered" );
    p = element;
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Remove
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::Remove( const unsigned long& identifier )
{
    elements_.erase( identifier );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Clear
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::Clear()
{
    elements_.clear();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MIL_KnowledgeGroup > KnowledgeGroupFactory::Find
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > KnowledgeGroupFactory::Find( const unsigned long& identifier ) const
{
    CIT_Elements it = elements_.find( identifier );
    if( it != elements_.end() )
        return it->second;
    return boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MIL_KnowledgeGroup > >& KnowledgeGroupFactory::GetElements
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
const std::map< unsigned long, boost::shared_ptr< MIL_KnowledgeGroup > >& KnowledgeGroupFactory::GetElements() const
{
    return elements_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::Count
// Created: MCO 2013-07-04
// -----------------------------------------------------------------------------
unsigned long KnowledgeGroupFactory::Count() const
{
    return (unsigned long)elements_.size();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::load
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object < KnowledgeGroupFactory_ABC >( *this );
    file >> elements_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupFactory::save
// Created: SLG 2010-02-10
// -----------------------------------------------------------------------------
void KnowledgeGroupFactory::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object < KnowledgeGroupFactory_ABC >( *this );
    file << elements_;
}
