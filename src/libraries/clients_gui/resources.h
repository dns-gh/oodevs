// File: resources.h
// 
// $Created:  CBX 01-06-22 $
//
//*****************************************************************************

#ifndef __demo_resources_h_
#define __demo_resources_h_

#define DECLARE_ICON( name )   extern const char *xpm_##name[]
#define MAKE_PIXMAP( name )   QPixmap( xpm_##name )
#define MAKE_ICON( name )   QIconSet( QPixmap( xpm_##name ) )
#define MAKE_ICON_X( name, name_disabled ) MakeExtendedIcon( xpm_##name, xpm_##name_disabled )

class QIconSet;

QIconSet MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] );

// Declare the icon here and include the xpm file in resources.cpp
DECLARE_ICON( csword );
DECLARE_ICON( connected   );
DECLARE_ICON( connecting );
DECLARE_ICON( notconnected );
DECLARE_ICON( play );
DECLARE_ICON( stop );
DECLARE_ICON( textsmall );
DECLARE_ICON( textbig );
DECLARE_ICON( crossings );
DECLARE_ICON( weather );
DECLARE_ICON( path );
DECLARE_ICON( oldpath );
DECLARE_ICON( embraye );
DECLARE_ICON( debraye );
DECLARE_ICON( visionlines );
DECLARE_ICON( visioncones );
DECLARE_ICON( visionsurfaces );
DECLARE_ICON( tickon );
DECLARE_ICON( tickoff );
DECLARE_ICON( tickred );
DECLARE_ICON( msg );
DECLARE_ICON( sim );
DECLARE_ICON( simdbg );
DECLARE_ICON( new );
DECLARE_ICON( open );
DECLARE_ICON( save );
DECLARE_ICON( saveas );
DECLARE_ICON( search );
DECLARE_ICON( ammo );
DECLARE_ICON( construction );
DECLARE_ICON( observe );
DECLARE_ICON( tacticallines );
DECLARE_ICON( threed );
DECLARE_ICON( visible );
DECLARE_ICON( select );
DECLARE_ICON( point );
DECLARE_ICON( line );
DECLARE_ICON( circle );
DECLARE_ICON( rectangle );
DECLARE_ICON( polygon );
DECLARE_ICON( text );
DECLARE_ICON( image );
DECLARE_ICON( empty );
DECLARE_ICON( cross );
DECLARE_ICON( saveall );
DECLARE_ICON( autolog );
DECLARE_ICON( missinglog );
DECLARE_ICON( loglink );
DECLARE_ICON( realtimelog );
DECLARE_ICON( cadenas_ouvert );
DECLARE_ICON( talkie );
DECLARE_ICON( aggregate );
DECLARE_ICON( desaggregate );
DECLARE_ICON( recrec );
DECLARE_ICON( recplay );
DECLARE_ICON( recstop );
DECLARE_ICON( flare );
DECLARE_ICON( smoke );
DECLARE_ICON( plus );
DECLARE_ICON( minus );
DECLARE_ICON( lock );
DECLARE_ICON( commandpost );
DECLARE_ICON( cancel );
DECLARE_ICON( option_general );
DECLARE_ICON( check );
DECLARE_ICON( check_grey );
DECLARE_ICON( profiles );
DECLARE_ICON( profile );
DECLARE_ICON( left_arrow );
DECLARE_ICON( right_arrow );
DECLARE_ICON( conflict );
DECLARE_ICON( filter );
DECLARE_ICON( pencil );
DECLARE_ICON( pencil_checkbox );
DECLARE_ICON( goto );
DECLARE_ICON( add_point );
DECLARE_ICON( special_point );
DECLARE_ICON( drawings );
DECLARE_ICON( arrow_up );
DECLARE_ICON( arrow_down );

#endif // __demo_resources_h_