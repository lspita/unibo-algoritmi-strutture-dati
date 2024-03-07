#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "libpsgraph.h"

static FILE *ps_file;
static double g_x, g_y;
static double dir;

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

void start( const char *filename )
{
    if ((ps_file = fopen( filename, "wt" )) == NULL) {
        fprintf(stderr, "Can not open %s. Stop\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf( ps_file, "%%!PS-Adobe-2.0\n" );
    g_x = 2*72;
    g_y = 7*72;
    fprintf( ps_file, "%d %d moveto\n", (int)(g_x), (int)(g_y) );
    dir = 0.0;
}

void end( void )
{
    fprintf( ps_file, "stroke\nshowpage\n" );
    fclose( ps_file );
}

void draw( double length )
{
    length /= 0.352777778;
    g_x += length * cos( dir );
    g_y += length * sin( dir );
    fprintf( ps_file, "%d %d lineto\n", (int)(g_x), (int)(g_y) );
}

void move( double length )
{
    length /= 0.352777778;
    g_x += length * cos( dir );
    g_y += length * sin( dir );
    fprintf( ps_file, "%d %d moveto\n", (int)(g_x), (int)(g_y) );
}

void turn( double ang )
{
    dir -= M_PI * ang / 180.0;
}

void save_state(Turtle *t)
{
    assert(t != NULL);
    t->g_x = g_x;
    t->g_y = g_y;
    t->dir = dir;
}

void setcolor( double r, double g, double b )
{
    fprintf(ps_file, "stroke\n%f %f %f setrgbcolor\n", r, g, b);
    move(0); /* to start a new path */
}

void restore_state(const Turtle *t)
{
    assert(t != NULL);
    g_x = t->g_x;
    g_y = t->g_y;
    dir = t->dir;
    fprintf( ps_file, "%d %d moveto\n", (int)g_x, (int)g_y);
}
