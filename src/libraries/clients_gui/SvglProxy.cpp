// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SvglProxy.h"
#include "SvglRenderer.h"
#include "tools/GeneralConfig.h"
#include <svgl/Node_ABC.h>
#include <xeumeuleu/xml.hpp>

using namespace gui;

SvglProxy::SvglProxy( SvglRenderer& renderer )
    : renderer_( renderer )
{
    // NOTHING
}

SvglProxy::~SvglProxy()
{
    // NOTHING
}

void SvglProxy::Draw( const std::string& name, const geometry::Rectangle2f& viewport,
                      unsigned vWidth /* = 640*/, unsigned vHeight /* = 480*/,
                      bool pickingMode /* = false*/ )
{
    auto& node = symbols_[ name ];
    if( !node )
    {
        tools::Xifstream input( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images" ) / tools::Path::FromUTF8( name ) ) );
        node.reset( renderer_.Compile( input, 100 ) ); // $$$$ AGE 2007-05-31:
    }
    if( node )
        renderer_.Render( node, SvglRenderer::DefaultStyle(), viewport, vWidth, vHeight, pickingMode );
}
