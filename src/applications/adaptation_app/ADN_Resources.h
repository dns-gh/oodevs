// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_h_
#define __ADN_Resources_h_

class QIconSet;

#define MAKE_PIXMAP( name ) QPixmap( xpm_##name )
#define MAKE_ICON( name )   QIconSet( QPixmap( xpm_##name ) )
#define DECLARE_ICON( name ) extern const char *xpm_##name[]

// Declaration
DECLARE_ICON( fileopen );
DECLARE_ICON( filesave );
DECLARE_ICON( filenew  );
DECLARE_ICON( testdata );
DECLARE_ICON( application );

#include "XmlResources.cpp"

#endif // __ADN_Resources_h_