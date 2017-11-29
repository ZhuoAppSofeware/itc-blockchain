# include <stdlib.h>
# include <stdio.h>
# include <time.h>

# include "rnglib.h"

/******************************************************************************/

void advance_state ( int k )
{
  const int a1 = 40014;
  const int a2 = 40692;
  int b1;
  int b2;
  int cg1;
  int cg2;
  int g;
  int i;
  const int m1 = 2147483563;
  const int m2 = 2147483399;

  if ( k < 0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "ADVANCE_STATE - Fatal error!\n" );
    fprintf ( stderr, "  Input exponent K is out of bounds.\n" );
    exit ( 1 );
  }
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "ADVANCE_STATE - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get the current generator index.
*/
  g = cgn_get ( );

  b1 = a1;
  b2 = a2;

  for ( i = 1; i <= k; k++ )
  {
    b1 = multmod ( b1, b1, m1 );
    b2 = multmod ( b2, b2, m2 );
  }

  cg_get ( g, &cg1, &cg2 );
  cg1 = multmod ( b1, cg1, m1 );
  cg2 = multmod ( b2, cg2, m2 );
  cg_set ( g, cg1, cg2 );

  return;
}
/******************************************************************************/

int antithetic_get ( )
{
  int i;
  int value;

  i = -1;
  antithetic_memory ( i, &value );

  return value;
}
/******************************************************************************/

void antithetic_memory ( int i, int *value )
{
# define G_MAX 32

  static int a_save[G_MAX];
  int g;
  const int g_max = 32;
  int j;

  if ( i < 0 )
  {
    g = cgn_get ( );
    *value = a_save[g];
  }
  else if ( i == 0 )
  {
    for ( j = 0; j < g_max; j++ )
    {
      a_save[j] = 0;
    }
  }
  else if ( 0 < i )
  {
    g = cgn_get ( );
    a_save[g] = *value;
  }

  return;
# undef G_MAX
}
/******************************************************************************/

void antithetic_set ( int value )
{
  int i;

  i = +1;
  antithetic_memory ( i, &value );

  return;
}
/******************************************************************************/

void cg_get ( int g, int *cg1, int *cg2 )
{
  int i;

  i = -1;
  cg_memory ( i, g, cg1, cg2 );

  return;
}
/******************************************************************************/

void cg_memory ( int i, int g, int *cg1, int *cg2 )
{
# define G_MAX 32

  static int cg1_save[G_MAX];
  static int cg2_save[G_MAX];
  const int g_max = 32;
  int j;

  if ( g < 0 || g_max <= g )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "CG_MEMORY - Fatal error!\n" );
    fprintf ( stderr, "  Input generator index G is out of bounds.\n" );
    exit ( 1 );
  }

  if ( i < 0 )
  {
    *cg1 = cg1_save[g];
    *cg2 = cg2_save[g];
  }
  else if ( i == 0 )
  {
    for ( j = 0; j < g_max; j++ )
    {
      cg1_save[j] = 0;
      cg2_save[j] = 0;
    }
  }
  else if ( 0 < i )
  {
    cg1_save[g] = *cg1;
    cg2_save[g] = *cg2;
  }

  return;
# undef G_MAX
}
/******************************************************************************/

void cg_set ( int g, int cg1, int cg2 )
{
  int i;

  i = +1;
  cg_memory ( i, g, &cg1, &cg2 );

  return;
}
/******************************************************************************/

int cgn_get ( )
{
  int g;
  int i;

  i = -1;
  cgn_memory ( i, &g );

  return g;
}
/******************************************************************************/

void cgn_memory ( int i, int *g )
{
# define G_MAX 32

  static int g_save = 0;
  const int g_max = 32;
  int j;

  if ( i < 0 )
  {
    *g = g_save;
  }
  else if ( i == 0 )
  {
    g_save = 0;
    *g = g_save;
  }
  else if ( 0 < i )
  {

    if ( *g < 0 || g_max <= *g )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "CGN_MEMORY - Fatal error!\n" );
      fprintf ( stderr, "  Input generator index G is out of bounds.\n" );
      exit ( 1 );
    }

    g_save = *g;
  }

  return;
# undef G_MAX
}
/******************************************************************************/

void cgn_set ( int g )
{
  int i;

  i = +1;
  cgn_memory ( i, &g );

  return;
}
/******************************************************************************/

void get_state ( int *cg1, int *cg2 )
{
  int g;
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "GET_STATE - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get the current generator index.
*/
  g = cgn_get ( );
/*
  Retrieve the seed values for this generator.
*/
  cg_get ( g, cg1, cg2 );

  return;
}
/******************************************************************************/

int i4_uni ( )
{
  const int a1 = 40014;
  const int a2 = 40692;
  int cg1;
  int cg2;
  int g;
  int k;
  const int m1 = 2147483563;
  const int m2 = 2147483399;
  int value;
  int z;
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "I4_UNI - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get the current generator index.
*/
  g = cgn_get ( );
/*
  Retrieve the current seeds.
*/
  cg_get ( g, &cg1, &cg2 );
/*
  Update the seeds.
*/
  k = cg1 / 53668;
  cg1 = a1 * ( cg1 - k * 53668 ) - k * 12211;

  if ( cg1 < 0 )
  {
    cg1 = cg1 + m1;
  }

  k = cg2 / 52774;
  cg2 = a2 * ( cg2 - k * 52774 ) - k * 3791;

  if ( cg2 < 0 )
  {
    cg2 = cg2 + m2;
  }
/*
  Store the updated seeds.
*/
  cg_set ( g, cg1, cg2 );
/*
  Form the random integer.
*/
  z = cg1 - cg2;

  if ( z < 1 )
  {
    z = z + m1 - 1;
  }
/*
  If the generator is antithetic, reflect the value.
*/
  value = antithetic_get ( );

  if ( value )
  {
    z = m1 - z;
  }
  return z;
}
/******************************************************************************/

void ig_get ( int g, int *ig1, int *ig2 )
{
  int i;

  i = -1;
  ig_memory ( i, g, ig1, ig2 );

  return;
}
/******************************************************************************/

void ig_memory ( int i, int g, int *ig1, int *ig2 )
{
# define G_MAX 32

  const int g_max = 32;
  static int ig1_save[G_MAX];
  static int ig2_save[G_MAX];
  int j;

  if ( g < 0 || g_max <= g )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "IG_MEMORY - Fatal error!\n" );
    fprintf ( stderr, "  Input generator index G is out of bounds.\n" );
    exit ( 1 );
  }

  if ( i < 0 )
  {
    *ig1 = ig1_save[g];
    *ig2 = ig2_save[g];
  }
  else if ( i == 0 )
  {
    for ( j = 0; j < g_max; j++ )
    {
      ig1_save[j] = 0;
      ig2_save[j] = 0;
    }
  }
  else if ( 0 < i )
  {
    ig1_save[g] = *ig1;
    ig2_save[g] = *ig2;
  }

  return;
# undef G_MAX
}
/******************************************************************************/

void ig_set ( int g, int ig1, int ig2 )
{
  int i;

  i = +1;
  ig_memory ( i, g, &ig1, &ig2 );

  return;
}
/******************************************************************************/

void init_generator ( int t )
{
  const int a1_w = 1033780774;
  const int a2_w = 1494757890;
  int cg1;
  int cg2;
  int g;
  int ig1;
  int ig2;
  int lg1;
  int lg2;
  const int m1 = 2147483563;
  const int m2 = 2147483399;
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "INIT_GENERATOR - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get the current generator index.
*/
  g = cgn_get ( );
/*
  0: restore the initial seed.
*/
  if ( t == 0 )
  {
    ig_get ( g, &ig1, &ig2 );
    lg1 = ig1;
    lg2 = ig2;
    lg_set ( g, lg1, lg2 );
  }
/*
  1: restore the last seed.
*/
  else if ( t == 1 )
  {
    lg_get ( g, &lg1, &lg2 );
  }
/*
  2: advance to a new seed.
*/
  else if ( t == 2 )
  {
    lg_get ( g, &lg1, &lg2 );
    lg1 = multmod ( a1_w, lg1, m1 );
    lg2 = multmod ( a2_w, lg2, m2 );
    lg_set ( g, lg1, lg2 );
  }
  else
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "INIT_GENERATOR - Fatal error!\n" );
    fprintf ( stderr, "  Input parameter T out of bounds.\n" );
    exit ( 1 );
  }
/*
  Store the new seed.
*/
  cg1 = lg1;
  cg2 = lg2;
  cg_set ( g, cg1, cg2 );

  return;
}
/******************************************************************************/

void initialize ( )
{
  int g;
  const int g_max = 32;
  int ig1;
  int ig2;
  int value;
/*
  Remember that we have called INITIALIZE().
*/
  initialized_set ( );
/*
  Initialize all generators to have FALSE antithetic value.
*/
  value = 0;
  for ( g = 0; g < g_max; g++ )
  {
    cgn_set ( g );
    antithetic_set ( value );
  }
/*
  Set the initial seeds.
*/
  ig1 = 1234567890;
  ig2 = 123456789;
  set_initial_seed ( ig1, ig2 );
/*
  Initialize the current generator index to 0.
*/
  g = 0;
  cgn_set ( g );

  printf ( "\n" );
  printf ( "INITIALIZE - Note:\n" );
  printf ( "  The RNGLIB package has been initialized.\n" );

  return;
}
/******************************************************************************/

int initialized_get ( )
{
  int i;
  int value;

  i = -1;
  initialized_memory ( i, &value );

  return value;
}
/******************************************************************************/

void initialized_memory ( int i, int *initialized )
{
  static int initialized_save = 0;

  if ( i < 0 )
  {
    *initialized = initialized_save;
  }
  else if ( i == 0 )
  {
    initialized_save = 0;
  }
  else if ( 0 < i )
  {
    initialized_save = *initialized;
  }

  return;
}
/******************************************************************************/

void initialized_set ( )
{
  int i;
  int initialized;

  i = +1;
  initialized = 1;
  initialized_memory ( i, &initialized );

  return;
}
/******************************************************************************/

void lg_get ( int g, int *lg1, int *lg2 )
{
  int i;

  i = -1;
  lg_memory ( i, g, lg1, lg2 );

  return;
}
/******************************************************************************/

void lg_memory ( int i, int g, int *lg1, int *lg2 )
{
# define G_MAX 32

  const int g_max = 32;

  int j;
  static int lg1_save[G_MAX];
  static int lg2_save[G_MAX];

  if ( g < 0 || g_max <= g )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "LG_MEMORY - Fatal error!\n" );
    fprintf ( stderr, "  Input generator index G is out of bounds.\n" );
    exit ( 1 );
  }

  if ( i < 0 )
  {
    *lg1 = lg1_save[g];
    *lg2 = lg2_save[g];
  }
  else if ( i == 0 )
  {
    for ( j = 0; j < g_max; j++ )
    {
      lg1_save[j] = 0;
      lg2_save[j] = 0;
    }
  }
  else if ( 0 < i )
  {
    lg1_save[g] = *lg1;
    lg2_save[g] = *lg2;
  }
  return;
# undef G_MAX
}
/******************************************************************************/

void lg_set ( int g, int lg1, int lg2 )
{
  int i;

  i = +1;
  lg_memory ( i, g, &lg1, &lg2 );

  return;
}
/******************************************************************************/

int multmod ( int a, int s, int m )
{
  int a0;
  int a1;
  const int h = 32768;
  int k;
  int p;
  int q;
  int qh;
  int rh;

  if ( a <= 0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "MULTMOD - Fatal error!\n" );
    fprintf ( stderr, "  A <= 0.\n" );
    exit ( 1 );
  }

  if ( m <= a )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "MULTMOD - Fatal error!\n" );
    fprintf ( stderr, "  M <= A.\n" );
    exit ( 1 );
  }

  if ( s <= 0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "MULTMOD - Fatal error!\n" );
    fprintf ( stderr, "  S <= 0.\n" );
    exit ( 1 );
  }

  if ( m <= s )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "MULTMOD - Fatal error!\n" );
    fprintf ( stderr, "  M <= S.\n" );
    exit ( 1 );
  }

  if ( a < h )
  {
    a0 = a;
    p = 0;
  }
  else
  {
    a1 = a / h;
    a0 = a - h * a1;
    qh = m / h;
    rh = m - h * qh;

    if ( h <= a1 )
    {
      a1 = a1 - h;
      k = s / qh;
      p = h * ( s - k * qh ) - k * rh;

      while ( p < 0 )
      {
        p = p + m;
      }
    }
    else
    {
      p = 0;
    }

    if ( a1 != 0 )
    {
      q = m / a1;
      k = s / q;
      p = p - k * ( m - a1 * q );

      if ( 0 < p )
      {
        p = p - m;
      }

      p = p + a1 * ( s - k * q );

      while ( p < 0 )
      {
        p = p + m;
      }
    }

    k = p / qh;
    p = h * ( p - k * qh ) - k * rh;

    while ( p < 0 )
    {
      p = p + m;
    }
  }

  if ( a0 != 0 )
  {
    q = m / a0;
    k = s / q;
    p = p - k * ( m - a0 * q );

    if ( 0 < p )
    {
      p = p - m;
    }

    p = p + a0 * ( s - k * q );

    while ( p < 0 )
    {
      p = p + m;
    }
  }
  return p;
}
/******************************************************************************/

float r4_uni_01 ( )
{
  int i;
  float value;
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "R4_UNI_01 - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get a random integer.
*/
  i = i4_uni ( );
/*
  Scale it to [0,1].
*/
  value = ( float ) ( i ) * 4.656613057E-10;

  return value;
}
/******************************************************************************/

double r8_uni_01 ( )
{
  int i;
  double value;
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "R8_UNI_01 - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Get a random integer.
*/
  i = i4_uni ( );
/*
  Scale it to [0,1].
*/
  value = ( double ) ( i ) * 4.656613057E-10;

  return value;
}
/******************************************************************************/

void set_initial_seed ( int ig1, int ig2 )
{
  const int a1_vw = 2082007225;
  const int a2_vw = 784306273;
  int g;
  const int g_max = 32;
  int i;
  const int m1 = 2147483563;
  const int m2 = 2147483399;
  int t;

  if ( ig1 < 1 || m1 <= ig1 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SET_INITIAL_SEED - Fatal error!\n" );
    fprintf ( stderr, "  Input parameter IG1 out of bounds.\n" );
    exit ( 1 );
  }

  if ( ig2 < 1 || m2 <= ig2 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SET_INITIAL_SEED - Fatal error!\n" );
    fprintf ( stderr, "  Input parameter IG2 out of bounds.\n" );
    exit ( 1 );
  }
/*
  Because INITIALIZE calls SET_INITIAL_SEED, it's not easy to correct
  the error that arises if SET_INITIAL_SEED is called before INITIALIZE.
  So don't bother trying.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "SET_INITIAL_SEED - Fatal error!\n" );
    printf ( "  The RNGLIB package has not been initialized.\n" );
    exit ( 1 );
  }
/*
  Set the initial seed, then initialize the first generator.
*/
  g = 0;
  cgn_set ( g );

  ig_set ( g, ig1, ig2 );

  t = 0;
  init_generator ( t );
/*
  Now do similar operations for the other generators.
*/
  for ( g = 1; g < g_max; g++ )
  {
    cgn_set ( g );
    ig1 = multmod ( a1_vw, ig1, m1 );
    ig2 = multmod ( a2_vw, ig2, m2 );
    ig_set ( g, ig1, ig2 );
    init_generator ( t );
  }
/*
  Now choose the first generator.
*/
  g = 0;
  cgn_set ( g );

  return;
}
/******************************************************************************/

void set_seed ( int cg1, int cg2 )
{
  int g;
  int i;
  const int m1 = 2147483563;
  const int m2 = 2147483399;
  int t;

  if ( cg1 < 1 || m1 <= cg1 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SET_SEED - Fatal error!\n" );
    fprintf ( stderr, "  Input parameter CG1 out of bounds.\n" );
    exit ( 1 );
  }

  if ( cg2 < 1 || m2 <= cg2 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "SET_SEED - Fatal error!\n" );
    fprintf ( stderr, "  Input parameter CG2 out of bounds.\n" );
    exit ( 1 );
  }
/*
  Check whether the package must be initialized.
*/
  if ( ! initialized_get ( ) )
  {
    printf ( "\n" );
    printf ( "SET_SEED - Note:\n" );
    printf ( "  Initializing RNGLIB package.\n" );
    initialize ( );
  }
/*
  Retrieve the current generator index.
*/
  g = cgn_get ( );
/*
  Set the seeds.
*/
  cg_set ( g, cg1, cg2 );
/*
  Initialize the generator.
*/
  t = 0;
  init_generator ( t );

  return;
}
/******************************************************************************/

void timestamp ( )
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  printf ( "%s\n", time_buffer );

  return;
# undef TIME_SIZE
}

