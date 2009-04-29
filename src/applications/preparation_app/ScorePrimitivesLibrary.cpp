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
#include "indicators/Primitive.h"
#include "indicators/Primitives.h"
#include <boost/bind.hpp>

namespace
{
    bool FilterExtractors( const indicators::Primitive& primitive )
    {
        return primitive.GetCategory() == "extract";
    }
    bool FilterUserFunctions( const indicators::Primitive& primitive )
    {
        const std::string category = primitive.GetCategory();
        return category != "extract" && category != "result";
    }
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary constructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::ScorePrimitivesLibrary( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const indicators::Primitives& primitives )
    : QTabWidget( parent )
{
    AddPage( tr( "Data" )     , new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( &::FilterExtractors, _1 ) ) );
    AddPage( tr( "Functions" ), new ScorePrimitivesPage( this, controllers, factory, primitives, boost::bind( &::FilterUserFunctions, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary destructor
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
ScorePrimitivesLibrary::~ScorePrimitivesLibrary()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScorePrimitivesLibrary::AddPage
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
void ScorePrimitivesLibrary::AddPage( const QString& name, QWidget* page )
{
    addTab( page, name );
    connect( page, SIGNAL( Selected( const indicators::Primitive& ) ), SIGNAL( Selected( const indicators::Primitive& ) ) );
    connect( page, SIGNAL( Insert( const QString& ) ), SIGNAL( Insert( const QString& ) ) );
}
