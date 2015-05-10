#include "Layer2D.h"

Layer2D::Layer2D( std::string name )
    : name_( name )
{
    // NOTHING
}

std::string Layer2D::GetName( ) const
{
    return name_;
}

unsigned int Layer2D::GetZOrder() const
{
    return zOrder_;
}