//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Resources.h $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_Resources.h $
//
//*****************************************************************************

#ifndef __ADN_Resources_h_
#define __ADN_Resources_h_

class ADN_ResourceXML;
class QIconSet;

QIconSet MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] );

#define MAKE_PIXMAP( name ) QPixmap( xpm_##name )
#define MAKE_ICON( name )   QIconSet( QPixmap( xpm_##name ) )
#define MAKE_ICON_X( name, name_disabled ) MakeExtendedIcon( xpm_##name, xpm_##name_disabled )
#define MAKE_XML( name ) ADN_ResourceXML(xml_##name)
#define DECLARE_ICON( name ) extern const char *xpm_##name[]

// Declaration

DECLARE_ICON( fileopen );
DECLARE_ICON( filesave );
DECLARE_ICON( filenew  );
DECLARE_ICON( testdata );



#endif // __ADN_Resources_h_