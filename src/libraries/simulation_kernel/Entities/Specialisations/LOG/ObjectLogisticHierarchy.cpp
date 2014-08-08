// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObjectLogisticHierarchy.h"

#include "MIL_AutomateLOG.h"
#include "ObjectLogisticLink.h"
#include "CheckPoints/SerializationTools.h"
#include <tools/Iterator.h>
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( logistic::ObjectLogisticHierarchy )

// =============================================================================
// $$$ A DEPLACER
// =============================================================================

namespace tools {

// =============================================================================
/** @class  SingleElementIterator
    @brief  Simple container reverse iterator
*/
// Created: AGE 2005-11-13
// =============================================================================
template< typename T >
class SingleElementIterator : public tools::Iterator_ABC< T >
{
public:
    SingleElementIterator( T item )
        : firstCall_( true )
        , item_     ( item )
    {}
    virtual bool HasMoreElements() const
    {
        return firstCall_;
    }

    virtual T NextElement()
    {
        if( firstCall_ )
        {
            firstCall_ = false;
            return item_;
        }
        throw MASA_EXCEPTION( "No more element" );
    }

    virtual void Rewind()
    {
        firstCall_ = true;
    }

private:
    SingleElementIterator( const SingleElementIterator& );
    SingleElementIterator& operator=( SingleElementIterator& );

    bool firstCall_;
    T item_;
};

}

using namespace logistic;

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
ObjectLogisticHierarchy::ObjectLogisticHierarchy()
    : superiorLink_( boost::shared_ptr< LogisticLink_ABC >() )
{
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy constructor
// Created: NLD 2011-01-05
// -----------------------------------------------------------------------------
ObjectLogisticHierarchy::ObjectLogisticHierarchy( MIL_AutomateLOG& superior )
    : superiorLink_( boost::make_shared< ObjectLogisticLink >( superior ) )
{
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy destructor
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
ObjectLogisticHierarchy::~ObjectLogisticHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::CreateSuperiorLinksIterator
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
tools::Iterator< boost::shared_ptr< LogisticLink_ABC > > ObjectLogisticHierarchy::CreateSuperiorLinksIterator() const
{
    return new tools::SingleElementIterator< boost::shared_ptr< LogisticLink_ABC > >( superiorLink_ );
}

// -----------------------------------------------------------------------------
// Name: tools::Iterator_ABC< MIL_AutomateLOG*> ObjectLogisticHierarchy::CreateSuperiorsIterator
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
tools::Iterator< MIL_AutomateLOG& > ObjectLogisticHierarchy::CreateSuperiorsIterator() const
{
    if( superiorLink_.get() )
        return new tools::SingleElementIterator< MIL_AutomateLOG& >( superiorLink_->GetSuperior() );
    else
        return new tools::NullIterator< MIL_AutomateLOG& >();
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::FindSuperiorLink
// Created: NLD 2011-01-11
// -----------------------------------------------------------------------------
const boost::shared_ptr< LogisticLink_ABC > ObjectLogisticHierarchy::FindSuperiorLink( const MIL_AutomateLOG& superior ) const
{
    if( superiorLink_.get() && superiorLink_->GetSuperior().GetLogisticId() == superior.GetLogisticId() )
        return superiorLink_;
    return boost::shared_ptr< LogisticLink_ABC >();
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::HasSuperior
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
bool ObjectLogisticHierarchy::HasSuperior() const
{
    return superiorLink_.get() != 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::GetPrimarySuperior
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
MIL_AutomateLOG* ObjectLogisticHierarchy::GetPrimarySuperior() const
{
    if( superiorLink_.get() )
        return &superiorLink_->GetSuperior();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::ChangeLinks
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void ObjectLogisticHierarchy::ChangeLinks( const std::vector< MIL_AutomateLOG* >& superiors )
{
    if( superiors.empty() )
        superiorLink_.reset();
    else
        superiorLink_.reset( new ObjectLogisticLink( *superiors.back() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::SwitchToHierarchy
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void ObjectLogisticHierarchy::SwitchToHierarchy( const LogisticHierarchy_ABC& )
{
    //$$$ TODO
    assert( false) ;
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::SwitchBackToNominalHierarchy
// Created: NLD 2011-01-12
// -----------------------------------------------------------------------------
void ObjectLogisticHierarchy::SwitchBackToNominalHierarchy()
{
    //$$$ TODO
    assert( false) ;
}

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::WriteODB
// Created: NLD 2011-02-03
// -----------------------------------------------------------------------------
void ObjectLogisticHierarchy::WriteODB( xml::xostream& ) const
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectLogisticHierarchy::serialize
// Created: NLD 2011-02-07
// -----------------------------------------------------------------------------
template < typename Archive >
void ObjectLogisticHierarchy::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< LogisticHierarchy_ABC >( *this )
         & superiorLink_;
}

