// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScorePrimitivesLibrary.h"
#include "moc_ScorePrimitivesLibrary.cpp"
#include "ScorePrimitivesPage.h"
#include "preparation/IndicatorPrimitive.h"
#include <boost/bind.hpp>

namespace
{
    bool FilterExtractors( const IndicatorPrimitive& primitive )
    {
        return primitive.GetCategory() == "extract";
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::ScorePrimitivesLibrary( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Resolver_ABC< IndicatorPrimitive, QString >& primitives )
    : QTabWidget( parent )
{
    {
        ScorePrimitivesPage* page = new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( &::FilterExtractors, _1 ) );
        addTab( page, tr( "Data" ) );
        connect( page, SIGNAL( Insert( const QString& ) ), SIGNAL( Insert( const QString& ) ) );
    }
    {
        ScorePrimitivesPage* page = new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( std::logical_not< bool >(), boost::bind( &::FilterExtractors, _1 ) ) );
        addTab( page, tr( "Functions" ) );
        connect( page, SIGNAL( Insert( const QString& ) ), SIGNAL( Insert( const QString& ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::~ScorePrimitivesLibrary()
{
    // NOTHING
}
