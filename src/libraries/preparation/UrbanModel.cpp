// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanModel.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include <boost/bind.hpp>
#include <urban/TerrainObject_ABC.h>
#include <urban/TerrainObjectVisitor_ABC.h>
#include <xeumeuleu/xml.h>

using namespace xml;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    Purge();
}

namespace
{
    class UrbanSendingCreationVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanSendingCreationVisitor( UrbanModel& model ) : model_( model )
        {}
        ~UrbanSendingCreationVisitor()
        {}
        virtual void VisitBlock( urban::TerrainObject_ABC& urbanObject )
        {
            model_.SendCreation( urbanObject );
        }
    private:
        UrbanModel& model_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
bool UrbanModel::Load( const std::string& directoryPath, urban::WorldParameters& world )
{
    Purge();
    bool ret = urban::Model::Load( directoryPath, world );
    if( ret )
    {
        UrbanSendingCreationVisitor visitor( *this );
        Accept( visitor );

    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Serialize
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::Serialize( const std::string& filename ) const
{
    if( filename.empty() )
        return;

    xml::xofstream xos( filename, xml::encoding( "ISO-8859-1" ) );
    xos << start( "urban-state" )
            << start( "blocks" );

    for( Resolver< gui::TerrainObjectProxy >::CIT_Elements it = Resolver< gui::TerrainObjectProxy >::elements_.begin(); it != Resolver< gui::TerrainObjectProxy >::elements_.end(); ++it )
    {
        xos << start( "block" );
            xos << attribute( "id", it->second->GetId() );
                xos << start( "capacities" );
                    xos << start( "structural" );
                        xos << attribute( "value", it->second->infrastructure_.structuralState_ );
                    xos << end();
                xos << end();
        xos << end();
    }

    xos     << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadUrbanState
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::LoadUrbanState( xml::xistream& xis )
{
    xis >> start( "urban-state" )
            >> start( "blocks" )
                >> xml::list( "block", boost::bind( &UrbanModel::ReadBlock, boost::ref( *this ), _1 ) )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadBlock
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadBlock( xistream& xis )
{
    unsigned int id;
    xis >> attribute( "id", id );
    gui::TerrainObjectProxy* proxy = Resolver< gui::TerrainObjectProxy >::Find( id );
    if( proxy )
    {
        xis >> optional() >> start( "capacities" )
                >> xml::list( *this, &UrbanModel::ReadCapacity, *proxy )
            >> end();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCapacity
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadCapacity( const std::string& capacity, xistream& xis, gui::TerrainObjectProxy& proxy )
{
    if( capacity == "structural" )
    {
        unsigned int value;
        xis >> attribute( "value", value );
        proxy.infrastructure_.structuralState_ = value;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    urban::Model::Purge();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SendCreation
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::SendCreation( urban::TerrainObject_ABC& urbanObject )
{
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controller_, urbanObject );
    controller_.Create( *pTerrainObject );
    if( !Resolver< gui::TerrainObjectProxy >::Find( urbanObject.GetId() ) )
        tools::Resolver< gui::TerrainObjectProxy >::Register( urbanObject.GetId(), *pTerrainObject );
}
