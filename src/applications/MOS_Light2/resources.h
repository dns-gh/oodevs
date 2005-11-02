//*****************************************************************************
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
DECLARE_ICON( mosicon );
DECLARE_ICON( connexiongreen   );
DECLARE_ICON( connexionorange );
DECLARE_ICON( connexionred );
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
DECLARE_ICON( gas );
DECLARE_ICON( ammo );
DECLARE_ICON( nbc );
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

#endif // __demo_resources_h_