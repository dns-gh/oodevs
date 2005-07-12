/*
	Written 1998-1999 by Uwe Langenkamp

	This software may be used and distributed according to the terms
	of the GNU Public License, incorporated herein by reference.

	The author may be reached as ul@it-guru.de
*/

/* History:
 *          98/13/07 Last Change
 *          98/29/03 
 *          98/28/03 Multiple Cursors Added
 *          98/21/03 Add Custom Cursor Bitmap
 *
 */

#include "MOS_Light_Pch.h"

#include <qpainter.h>
#include <qbitmap.h> 
#include <qpntarry.h>
#include <qapp.h>
#include <qsocknot.h>
#include <qpopmenu.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qkeycode.h>
#include <qtimer.h>
#include <qbttngrp.h>
#include <qfiledlg.h>
#include <qstrlist.h>
#include <qmsgbox.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "aibase.h"

void AIBase::setDesktopPal(QApplication *)
{
}

