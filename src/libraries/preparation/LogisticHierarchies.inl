// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

 #include  <boost/type_traits/remove_const.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
LogisticHierarchies< Superior, I >::LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity )
    : LogisticHierarchies_ABC< I >( controller, entity )
    , tc2_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
LogisticHierarchies< Superior, I >::~LogisticHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::Load
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::Load( xml::xistream& xis, const kernel::Entity_ABC* superior )
{
    assert( dynamic_cast<Superior::BaseType>(superior) );
    SetSuperior( static_cast<Superior::BaseType>(superior) );
    Load( xis );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::SetSuperior( const Superior& entity )
{
    const kernel::Entity_ABC* superior = entity;
    LogisticHierarchies_ABC< I >::SetSuperior( const_cast< kernel::Entity_ABC* >( superior ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperiorInternal
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::SetSuperiorInternal( kernel::Entity_ABC* superior )
{
    assert( superior==0 || dynamic_cast< boost::remove_const<typename Superior::BaseType>::type >( superior ) );
    tc2_ = static_cast< boost::remove_const<typename Superior::BaseType>::type >( superior );
    LogisticHierarchies_ABC< I >::SetSuperiorInternal( superior );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::DrawLink
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::DrawLink( const geometry::Point2f& where, const kernel::GlTools_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const
{
    if( tc2_ && displayLinks )
        tools.DrawCurvedArrow( where, tc2_->Get< kernel::Positions >().GetPosition(), curve );
    else if( ! tc2_ && displayMissings )
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SerializeLogistics
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename Superior, typename I >
void LogisticHierarchies< Superior, I >::SerializeLogistics( xml::xostream& xos ) const
{    
    tools::Iterator< const kernel::Entity_ABC& > children = CreateSubordinateIterator();
    if( !children.HasMoreElements() )
        return;

    xos << xml::start( GetLinkType().ascii() )
            << xml::attribute( "id", GetEntity().GetId());
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = children.NextElement();
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", entity.GetId());
                SerializeQuotas( xos );
        xos << xml::end;
    }
    xos << xml::end;
}

