/***************************************************************************
 *   Copyright (C) 2004-2012 by CEA LIST                               *
 *                                                                         *
 ***************************************************************************/

#include <common/LimaCommon.h>

#ifndef LIMA_COMPOUNDSEXPORT_H
#define LIMA_COMPOUNDSEXPORT_H

#ifdef WIN32

#ifdef LIMA_COMPOUNDS_EXPORTING
   #define LIMA_COMPOUNDS_EXPORT    __declspec(dllexport)
#else
   #define LIMA_COMPOUNDS_EXPORT    __declspec(dllimport)
#endif


#else // Not WIN32

#define LIMA_COMPOUNDS_EXPORT

#endif

#endif

