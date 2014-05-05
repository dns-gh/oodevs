// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

namespace gui
{
    // -----------------------------------------------------------------------------
    // Name: RichView_ABC::AddNumericFilter
    // Created: ABR 2012-06-20
    // -----------------------------------------------------------------------------
    template< typename NumericType, typename SpinBox >
    void RichView_ABC::AddNumericFilter( int col,
                                         int areas,
                                         const QString& text,
                                         const typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor& extractor,
                                         NumericType min /* = 0 */,
                                         NumericType max /* = 0 */ )
    {
        if( !IsValidArea( areas ) )
        {
            MT_LOG_ERROR_MSG( "Incompatible filter area and rich view options" );
            return;
        }
        SubObjectName subObject( "ResolverFilter" + text );
        if( areas & eFilterAreas_Box )
        {
            auto editor = CreateNumericEditor< NumericType, SpinBox >( this, extractor, min, max );
            AddWidgetToFilterBox( col, text, editor, editor );
        }
        if( areas & eFilterAreas_Menu )
        {
            auto editor = CreateNumericEditor< NumericType, SpinBox >( GetMenu( col ), extractor, min, max );
            AddWidgetToMenu( col, text, editor, editor );
        }
    }

    // -----------------------------------------------------------------------------
    // Name: RichView_ABC::CreateNumericEditor
    // Created: ABR 2014-04-29
    // -----------------------------------------------------------------------------
    template< typename NumericType, typename SpinBox >
    NumericLimitsEditor< NumericType, SpinBox >* RichView_ABC::CreateNumericEditor( QWidget* parent,
                                                                                    const typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor& extractor,
                                                                                    NumericType min,
                                                                                    NumericType max)
    {
        auto editor = new NumericLimitsEditor< NumericType, SpinBox >( parent, extractor, min, max );
        connect( editor, SIGNAL( ValueChanged() ), signalMapper_, SLOT( map() ) );
        signalMapper_->setMapping( editor, static_cast< QWidget* >( editor ) );
        return editor;
    }

} //! namespace gui
