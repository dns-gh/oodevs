//*****************************************************************************
// File: resources.h
// 
// $Created:  CBX 01-06-22 $
//
//*****************************************************************************

#ifndef __demo_resources_h_
#define __demo_resources_h_

#define DECLARE_ICON( name ) extern const char *xpm_##name[]

class QIconSet;

QIconSet MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] );

#define MAKE_PIXMAP( name ) QPixmap( xpm_##name )
#define MAKE_ICON( name )   QIconSet( QPixmap( xpm_##name ) )
#define MAKE_ICON_X( name, name_disabled ) MakeExtendedIcon( xpm_##name, xpm_##name_disabled )

// Declare the icon here and include the xpm file in resources.cpp

DECLARE_ICON( fileopen );
DECLARE_ICON( zoomin   );
DECLARE_ICON( zoomout  );
DECLARE_ICON( pause    );
DECLARE_ICON( play     );
DECLARE_ICON( stop     );
DECLARE_ICON( l_france );
DECLARE_ICON( l_red    );
DECLARE_ICON( l_white  );
DECLARE_ICON( l_green  );
DECLARE_ICON( l_play  );
DECLARE_ICON( l_stop  );
DECLARE_ICON( a_down );
DECLARE_ICON( a_left );
DECLARE_ICON( a_right );
DECLARE_ICON( a_up );
DECLARE_ICON( l_black );
DECLARE_ICON( affinity );

#endif // __demo_resources_h_