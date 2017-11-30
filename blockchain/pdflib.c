# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <string.h>

# include "pdflib.h"
# include "rnglib.h"

/******************************************************************************/

double i4_binomial_pdf ( int n, double p, int k )
{
  double value;

  if ( k < 0 )
  {
    value = 0.0;
  }
  else if ( k <= n )
  {
    value = r8_choose ( n, k ) * pow ( p, k ) * pow ( 1.0 - p, k );
  }
  else
  {
    value = 0.0;
  }
  return value;
}
/******************************************************************************/

int i4_binomial_sample ( int n, double pp )
{
  double al;
  double alv;
  double amaxp;
  double c;
  double f;
  double f1;
  double f2;
  double ffm;
  double fm;
  double g;
  int i;
  int ix;
  int ix1;
  int k;
  int m;
  int mp;
  double p;
  double p1;
  double p2;
  double p3;
  double p4;
  double q;
  double qn;
  double r;
  double t;
  double u;
  double v;
  int value;
  double w;
  double w2;
  double x;
  double x1;
  double x2;
  double xl;
  double xll;
  double xlr;
  double xm;
  double xnp;
  double xnpq;
  double xr;
  double ynorm;
  double z;
  double z2;

  if ( pp <= 0.0 || 1.0 <= pp )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "I4_BINOMIAL_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  PP is out of range.\n" );
    exit ( 1 );
  }

  p = r8_min ( pp, 1.0 - pp );
  q = 1.0 - p;
  xnp = ( double ) ( n ) * p;

  if ( xnp < 30.0 )
  {
    qn = pow ( q, n );
    r = p / q;
    g = r * ( double ) ( n + 1 );

    for ( ; ; )
    {
      ix = 0;
      f = qn;
      u = r8_uniform_01_sample ( );

      for ( ; ; )
      {
        if ( u < f )
        {
          if ( 0.5 < pp )
          {
            ix = n - ix;
          }
          value = ix;
          return value;
        }

        if ( 110 < ix )
        {
          break;
        }
        u = u - f;
        ix = ix + 1;
        f = f * ( g / ( double ) ( ix ) - r );
      }
    }
  }
  ffm = xnp + p;
  m = ffm;
  fm = m;
  xnpq = xnp * q;
  p1 = ( int ) ( 2.195 * sqrt ( xnpq ) - 4.6 * q ) + 0.5;
  xm = fm + 0.5;
  xl = xm - p1;
  xr = xm + p1;
  c = 0.134 + 20.5 / ( 15.3 + fm );
  al = ( ffm - xl ) / ( ffm - xl * p );
  xll = al * ( 1.0 + 0.5 * al );
  al = ( xr - ffm ) / ( xr * q );
  xlr = al * ( 1.0 + 0.5 * al );
  p2 = p1 * ( 1.0 + c + c );
  p3 = p2 + c / xll;
  p4 = p3 + c / xlr;
/*
  Generate a variate.
*/
  for ( ; ; )
  {
    u = r8_uniform_01_sample ( ) * p4;
    v = r8_uniform_01_sample ( );
/*
  Triangle
*/
    if ( u < p1 )
    {
      ix = xm - p1 * v + u;
      if ( 0.5 < pp ) 
      {
        ix = n - ix;
      }
      value = ix;
      return value;
    }
/*
  Parallelogram
*/
    if ( u <= p2 )
    {
      x = xl + ( u - p1 ) / c;
      v = v * c + 1.0 - fabs ( xm - x ) / p1;

      if ( v <= 0.0 || 1.0 < v )
      {
        continue;
      }
      ix = x;
    }
    else if ( u <= p3 )
    {
      ix = xl + log ( v ) / xll;
      if ( ix < 0 )
      {
        continue;
      }
      v = v * ( u - p2 ) * xll;
    }
    else
    {
      ix = xr - log ( v ) / xlr;
      if ( n < ix )
      {
        continue;
      }
      v = v * ( u - p3 ) * xlr;
    }
    k = abs ( ix - m );

    if ( k <= 20 || xnpq / 2.0 - 1.0 <= k )
    {
      f = 1.0;
      r = p / q;
      g = ( n + 1 ) * r;

      if ( m < ix )
      {
        mp = m + 1;
        for ( i = mp; i <= ix; i++ )
        {
          f = f * ( g / i - r );
        }
      }
      else if ( ix < m )
      {
        ix1 = ix + 1;
        for ( i = ix1; i <= m; i++ )
        {
          f = f / ( g / i - r );
        }
      }

      if ( v <= f )
      {
        if ( 0.5 < pp )
        {
          ix = n - ix;
        }
        value = ix;
        return value;
      }
    }
    else
    {
      amaxp = ( k / xnpq ) * ( ( k * ( k / 3.0 
        + 0.625 ) + 0.1666666666666 ) / xnpq + 0.5 );
      ynorm = - ( double ) ( k * k ) / ( 2.0 * xnpq );
      alv = log ( v );

      if ( alv < ynorm - amaxp )
      {
        if ( 0.5 < pp )
        {
          ix = n - ix;
        }
        value = ix;
        return value;
      }

      if ( ynorm + amaxp < alv )
      {
        continue;
      }

      x1 = ( double ) ( ix + 1 );
      f1 = fm + 1.0;
      z = ( double ) ( n + 1 ) - fm;
      w = ( double ) ( n - ix + 1 );
      z2 = z * z;
      x2 = x1 * x1;
      f2 = f1 * f1;
      w2 = w * w;

      t = xm * log ( f1 / x1 ) + ( n - m + 0.5 ) * log ( z / w ) 
        + ( double ) ( ix - m ) * log ( w * p / ( x1 * q )) 
        + ( 13860.0 - ( 462.0 - ( 132.0 - ( 99.0 - 140.0 
        / f2 ) / f2 ) / f2 ) / f2 ) / f1 / 166320.0 
        + ( 13860.0 - ( 462.0 - ( 132.0 - ( 99.0 - 140.0 
        / z2 ) / z2 ) / z2 ) / z2 ) / z / 166320.0 
        + ( 13860.0 - ( 462.0 - ( 132.0 - ( 99.0 - 140.0 
        / x2 ) / x2 ) / x2 ) / x2 ) / x1 / 166320.0 
        + ( 13860.0 - ( 462.0 - ( 132.0 - ( 99.0 - 140.0 
        / w2 ) / w2 ) / w2 ) / w2 ) / w / 166320.0;

      if ( alv <= t )
      {
        if ( 0.5 < pp )
        {
          ix = n - ix;
        }
        value = ix;
        return value;
      }
    }
  }
  return value;
}
/******************************************************************************/

int i4_max ( int i1, int i2 )
{
  int value;

  if ( i2 < i1 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
/******************************************************************************/

int i4_min ( int i1, int i2 )
{
  int value;

  if ( i1 < i2 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}
/******************************************************************************/

double i4vec_multinomial_pdf ( int n, double p[], int m, int x[] )
{
  int bot;
  int c;
  int i;
  int j;
  double pdf;
  int top;
/*
  The combinatorial coefficient is an integer.
*/
  c = 1;
  top = n;
  for ( i = 0; i < m; i++ )
  {
    bot = 1;
    for ( j = 0; j < x[i]; j++ )
    {
      c = ( c * top ) / bot;
      top = top - 1;
      bot = bot + 1;
    }
  }

  pdf = ( double ) ( c );
  for ( i = 0; i < m; i++ )
  {
    pdf = pdf * pow ( p[i], x[i] );
  }

  return pdf;
}
/******************************************************************************/

int *i4vec_multinomial_sample ( int n, double p[], int ncat )
{
  int i;
  int icat;
  int *ix;
  int ntot;
  double prob;
  double ptot;

  if ( n < 0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "I4VEC_MULTINOMIAL_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  N < 0\n" );
    exit ( 1 );
  }

  if ( ncat <= 1 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "I4VEC_MULTINOMIAL_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  NCAT <= 1\n" );
    exit ( 1 );
  }

  for ( i = 0; i < ncat - 1; i++ )
  {
    if ( p[i] < 0.0 )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "I4VEC_MULTINOMIAL_SAMPLE - Fatal error!\n" );
      fprintf ( stderr, "  Some P(i) < 0.\n" );
      exit ( 1 );
    }

    if ( 1.0 < p[i] )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "I4VEC_MULTINOMIAL_SAMPLE - Fatal error!\n" );
      fprintf ( stderr, "  Some 1 < P(i).\n" );
      exit ( 1 );
    }
  }

  ptot = 0.0;
  for ( i = 0; i < ncat - 1; i++ )
  {
    ptot = ptot + p[i];
  }

  if ( 0.99999 < ptot ) 
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "I4VEC_MULTINOMIAL_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  1.0 < Sum of P().\n" );
    exit ( 1 );
  }
/*
  Initialize variables.
*/
  ntot = n;
  ptot = 1.0;

  ix = ( int * ) malloc ( ncat * sizeof ( int ) );
  for ( i = 0; i < ncat; i++ )
  {
    ix[i] = 0;
  }
/*
  Generate the observation.
*/
  for ( icat = 0; icat < ncat - 1; icat++ )
  {
    prob = p[icat] / ptot;
    ix[icat] = i4_binomial_sample ( ntot, prob );
    ntot = ntot - ix[icat];
    if ( ntot <= 0 )
    {
      return ix;
    }
    ptot = ptot - p[icat];
  }

  ix[ncat-1] = ntot;

  return ix;
}
/******************************************************************************/

double r8_beta_pdf ( double alpha, double beta, double rval )
{
  double temp;
  double value;

  if ( alpha <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_BETA_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter ALPHA is not positive.\n" );
    exit ( 1 );
  }

  if ( beta <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_BETA_PDF- Fatal error!\n" );
    fprintf ( stderr, "  Parameter BETA is not positive.\n" );
    exit ( 1 );
  }

  if ( rval <= 0.0 || 1.0 <= rval )
  {
    value = 0.0;
  }
  else
  {
    temp = r8_gamma_log ( alpha + beta ) - r8_gamma_log ( alpha ) 
      - r8_gamma_log ( beta );

    value = exp ( temp ) * pow ( rval, alpha - 1.0 )
      * pow ( 1.0 - rval, beta - 1.0 );
  }
  return value;
}
/******************************************************************************/

double r8_beta_sample ( double aa, double bb )
{
  double a;
  double alpha;
  double b;
  double beta;
  double delta;
  double gamma;
  double k1;
  double k2;
  const double log4 = 1.3862943611198906188;
  const double log5 = 1.6094379124341003746;
  double r;
  double s;
  double t;
  double u1;
  double u2;
  double v;
  double value;
  double w;
  double y;
  double z;

  if ( aa <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_BETA_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  AA <= 0.0\n" );
    exit ( 1 );
  }

  if ( bb <= 0.0 ) 
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_BETA_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  BB <= 0.0\n" );
    exit ( 1 );
  }
/*
  Algorithm BB
*/
  if ( 1.0 < aa && 1.0 < bb )
  {
    if ( aa < bb )
    {
      a = aa;
      b = bb;
    }
    else
    {
      a = bb;
      b = aa;
    }
    alpha = a + b;
    beta = sqrt ( ( alpha - 2.0 ) / ( 2.0 * a * b - alpha ) );
    gamma = a + 1.0 / beta;

    for ( ; ; )
    {
      u1 = r8_uniform_01_sample ( );
      u2 = r8_uniform_01_sample ( );
      v = beta * log ( u1 / ( 1.0 - u1 ) );
      w = a * exp ( v );

      z = u1 * u1 * u2;
      r = gamma * v - log4;
      s = a + r - w;

      if ( 5.0 * z <= s + 1.0 + log5 )
      {
        break;
      }

      t = log ( z );
      if ( t <= s )
      {
        break;
      }

      if ( t <= ( r + alpha * log ( alpha / ( b + w ) ) ) )
      {
        break;
      }
    }
  }
/*
  Algorithm BC
*/
  else
  {
    if ( aa < bb )
    {
      a = bb;
      b = aa;
    }
    else
    {
      a = aa;
      b = bb;
    }

    alpha = a + b;
    beta = 1.0 / b;
    delta = 1.0 + a - b;
    k1 = delta * ( 1.0 / 72.0 + b / 24.0 ) 
      / ( a / b - 7.0 / 9.0 );
    k2 = 0.25 + ( 0.5 + 0.25 / delta ) * b;

    for ( ; ; )
    {
      u1 = r8_uniform_01_sample ( );
      u2 = r8_uniform_01_sample ( );

      if ( u1 < 0.5 )
      {
        y = u1 * u2;
        z = u1 * y;

        if ( k1 <= 0.25 * u2 + z - y )
        {
          continue;
        }
      }
      else
      {
        z = u1 * u1 * u2;

        if ( z <= 0.25 )
        {
          v = beta * log ( u1 / ( 1.0 - u1 ) );
          w = a * exp ( v );

          if ( aa == a )
          {
            value = w / ( b + w );
          }
          else
          {
            value = b / ( b + w );
          }
          return value;
        }

        if ( k2 < z )
        {
          continue;
        }
      }

      v = beta * log ( u1 / ( 1.0 - u1 ) );
      w = a * exp ( v );

      if ( log ( z ) <= alpha * ( log ( alpha / ( b + w ) ) + v ) - log4 )
      {
        break;
      }
    }
  }

  if ( aa == a )
  {
    value = w / ( b + w );
  }
  else
  {
    value = b / ( b + w );
  }
  return value;
}
/******************************************************************************/

double r8_chi_pdf ( double df, double rval )
{
  double temp1;
  double temp2;
  double value;

  if ( df <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_CHI_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Degrees of freedom must be positive.\n" );
    exit ( 1 );
  }
      
  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp2 = df * 0.5;

    temp1 = ( temp2 - 1.0 ) * log ( rval ) - 0.5 * rval 
      - temp2 * log ( 2.0 ) - r8_gamma_log ( temp2 );

    value = exp ( temp1 );
  }
  return value;
}
/******************************************************************************/

double r8_chi_sample ( double df )
{
  double arg1;
  double arg2;
  double value;

  if ( df <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_CHI_SAMPLE - Fatal error!\n" );
    fprintf ( stderr, "  DF <= 0.\n" );
    fprintf ( stderr, "  Value of DF: %g\n", df );
    exit ( 1 );
  }

  arg1 = 1.0;
  arg2 = df / 2.0;

  value = 2.0 * r8_gamma_sample ( arg1, arg2 );

  return value;
}
/******************************************************************************/

double r8_choose ( int n, int k )
{
  int i;
  int mn;
  int mx;
  double value;

  if ( k < n - k )
  {
    mn = k;
    mx = n - k;
  }
  else
  {
    mn = n - k;
    mx = k;
  }

  if ( mn < 0 )
  {
    value = 0.0;
  }
  else if ( mn == 0 )
  {
    value = 1.0;
  }
  else
  {
    value = ( double ) ( mx + 1 );

    for ( i = 2; i <= mn; i++ )
    {
      value = ( value * ( double ) ( mx + i ) ) / ( double ) i;
    }
  }

  return value;
}
/******************************************************************************/

double r8_epsilon ( )
{
  const double value = 2.220446049250313E-016;

  return value;
}
/******************************************************************************/

double r8_exponential_pdf ( double beta, double rval )
{
  double value;

  if ( beta <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_EXPONENTIAL_PDF - Fatal error!\n" );
    fprintf ( stderr, "  BETA parameter must be positive.\n" );
    exit ( 1 );
  }

  if ( rval < 0.0 )
  {
    value = 0.0;
  }
  else
  {
    value = exp ( - rval / beta ) / beta;
  }

  return value;
}
/******************************************************************************/

double r8_exponential_sample ( double lambda )
{
  double r;
  double value;

  r = r8_uniform_01_sample ( );

  value = - log ( r ) * lambda;

  return value;
}
/******************************************************************************/

double r8_exponential_01_pdf ( double rval )
{
  double value;

  if ( rval < 0.0 )
  {
    value = 0.0;
  }
  else
  {
    value = exp ( - rval );
  }

  return value;
}
/******************************************************************************/

double r8_exponential_01_sample ( )
{
  double r;
  double value;

  r = r8_uniform_01_sample ( );

  value = - log ( r );

  return value;
}
/******************************************************************************/

double r8_gamma_log ( double x )
{
  double c[7] = {
    -1.910444077728E-03, 
     8.4171387781295E-04, 
    -5.952379913043012E-04,
     7.93650793500350248E-04, 
    -2.777777777777681622553E-03, 
     8.333333333333333331554247E-02, 
     5.7083835261E-03 };
  double corr;
  const double d1 = -5.772156649015328605195174E-01;
  const double d2 = 4.227843350984671393993777E-01;
  const double d4 = 1.791759469228055000094023;
  const double frtbig = 2.25E+76;
  int i;
  double p1[8] = {
    4.945235359296727046734888, 
    2.018112620856775083915565E+02, 
    2.290838373831346393026739E+03, 
    1.131967205903380828685045E+04, 
    2.855724635671635335736389E+04, 
    3.848496228443793359990269E+04, 
    2.637748787624195437963534E+04, 
    7.225813979700288197698961E+03 };
  double p2[8] = { 
    4.974607845568932035012064, 
    5.424138599891070494101986E+02, 
    1.550693864978364947665077E+04, 
    1.847932904445632425417223E+05, 
    1.088204769468828767498470E+06, 
    3.338152967987029735917223E+06, 
    5.106661678927352456275255E+06, 
    3.074109054850539556250927E+06 };
  double p4[8] = {
    1.474502166059939948905062E+04, 
    2.426813369486704502836312E+06, 
    1.214755574045093227939592E+08, 
    2.663432449630976949898078E+09, 
    2.940378956634553899906876E+10, 
    1.702665737765398868392998E+11, 
    4.926125793377430887588120E+11, 
    5.606251856223951465078242E+11 };
  double q1[8] = { 
    6.748212550303777196073036E+01, 
    1.113332393857199323513008E+03, 
    7.738757056935398733233834E+03, 
    2.763987074403340708898585E+04, 
    5.499310206226157329794414E+04, 
    6.161122180066002127833352E+04, 
    3.635127591501940507276287E+04, 
    8.785536302431013170870835E+03 };
  double q2[8] = { 
    1.830328399370592604055942E+02, 
    7.765049321445005871323047E+03, 
    1.331903827966074194402448E+05, 
    1.136705821321969608938755E+06, 
    5.267964117437946917577538E+06, 
    1.346701454311101692290052E+07, 
    1.782736530353274213975932E+07, 
    9.533095591844353613395747E+06 };
  double q4[8] = { 
    2.690530175870899333379843E+03, 
    6.393885654300092398984238E+05, 
    4.135599930241388052042842E+07, 
    1.120872109616147941376570E+09, 
    1.488613728678813811542398E+10, 
    1.016803586272438228077304E+11, 
    3.417476345507377132798597E+11, 
    4.463158187419713286462081E+11 };
  double res;
  const double sqrtpi = 0.9189385332046727417803297;
  const double xbig = 2.55E+305;
  double xden;
  const double xinf = 1.79E+308;
  double xm1;
  double xm2;
  double xm4;
  double xnum;
  double y;
  double ysq;

  y = x;

  if ( 0.0 < y && y <= xbig )
  {
    if ( y <= r8_epsilon ( ) )
    {
      res = - log ( y );
    }
/*
  EPS < X <= 1.5.
*/
    else if ( y <= 1.5 )
    {
      if ( y < 0.6796875 )
      {
        corr = -log ( y );
        xm1 = y;
      }
      else
      {
        corr = 0.0;
        xm1 = ( y - 0.5 ) - 0.5;
      }

      if ( y <= 0.5 || 0.6796875 <= y )
      {
        xden = 1.0;
        xnum = 0.0;
        for ( i = 0; i < 8; i++ )
        {
          xnum = xnum * xm1 + p1[i];
          xden = xden * xm1 + q1[i];
        }
        res = corr + ( xm1 * ( d1 + xm1 * ( xnum / xden ) ) );
      }
      else
      {
        xm2 = ( y - 0.5 ) - 0.5;
        xden = 1.0;
        xnum = 0.0;
        for ( i = 0; i < 8; i++ )
        {
          xnum = xnum * xm2 + p2[i];
          xden = xden * xm2 + q2[i];
        }
        res = corr + xm2 * ( d2 + xm2 * ( xnum / xden ) );
      }
    }
/*
  1.5 < X <= 4.0.
*/
    else if ( y <= 4.0 )
    {
      xm2 = y - 2.0;
      xden = 1.0;
      xnum = 0.0;
      for ( i = 0; i < 8; i++ )
      {
        xnum = xnum * xm2 + p2[i];
        xden = xden * xm2 + q2[i];
      }
      res = xm2 * ( d2 + xm2 * ( xnum / xden ) );
    }
/*
  4.0 < X <= 12.0.
*/
    else if ( y <= 12.0 )
    {
      xm4 = y - 4.0;
      xden = -1.0;
      xnum = 0.0;
      for ( i = 0; i < 8; i++ )
      {
        xnum = xnum * xm4 + p4[i];
        xden = xden * xm4 + q4[i];
      }
      res = d4 + xm4 * ( xnum / xden );
    }
/*
  Evaluate for 12 <= argument.
*/
    else
    {
      res = 0.0;

      if ( y <= frtbig )
      {
        res = c[6];
        ysq = y * y;
        for ( i = 0; i < 6; i++ )
        {
          res = res / ysq + c[i];
        }
      }
      res = res / y;
      corr = log ( y );
      res = res + sqrtpi - 0.5 * corr;
      res = res + y * ( corr - 1.0 );
    }
  }
/*
  Return for bad arguments.
*/
  else
  {
    res = xinf;
  }
/*
  Final adjustments and return.
*/
  return res;
}
/******************************************************************************/

double r8_gamma_pdf ( double beta, double alpha, double rval )
{
  double temp;
  double value;

  if ( alpha <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_GAMMA_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter ALPHA is not positive.\n" );
    exit ( 1 );
  }

  if ( beta <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_GAMMA_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter BETA is not positive.\n" );
    exit ( 1 );
  }

  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp = alpha * log ( beta ) + ( alpha - 1.0 ) * log ( rval ) 
      - beta * rval - r8_gamma_log ( alpha );

    value = exp ( temp );
  }

  return value;
}
/******************************************************************************/

double r8_gamma_sample ( double a, double r )
{
  double value;

  value = r8_gamma_01_sample ( r ) / a;

  return value;
}
/******************************************************************************/

double r8_gamma_01_pdf ( double alpha, double rval )
{
  double temp;
  double value;

  if ( alpha <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_GAMMA_01_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter ALPHA is not positive.\n" );
    exit ( 1 );
  }

  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp = ( alpha - 1.0 ) * log ( rval ) - rval - r8_gamma_log ( alpha );

    value = exp ( temp );
  }

  return value;
}
/******************************************************************************/

double r8_gamma_01_sample ( double a )
{
  double a1 =  0.3333333;
  double a2 = -0.2500030;
  double a3 =  0.2000062;
  double a4 = -0.1662921;
  double a5 =  0.1423657;
  double a6 = -0.1367177;
  double a7 =  0.1233795;
  double b;
  double c;
  double d;
  double e;
  double e1 = 1.0;
  double e2 = 0.4999897;
  double e3 = 0.1668290;
  double e4 = 0.0407753;
  double e5 = 0.0102930;
  double p;
  double q;
  double q0;
  double q1 =  0.04166669;
  double q2 =  0.02083148;
  double q3 =  0.00801191;
  double q4 =  0.00144121;
  double q5 = -0.00007388;
  double q6 =  0.00024511;
  double q7 =  0.00024240;
  double r;
  double s;
  double s2;
  double si;
  double sqrt32 = 5.6568542494923801952;
  double t;
  double u;
  double v;
  double value;
  double w;
  double x;

  if ( 1.0 <= a )
  {
    s2 = a - 0.5;
    s = sqrt ( s2 );
    d = sqrt32 - 12.0 * s;
/*
  Immediate acceptance.
*/
    t = r8_normal_01_sample ( );
    x = s + 0.5 * t;
    value = x * x;

    if ( 0.0 <= t )
    {
      return value;
    }
/*
  Squeeze acceptance.
*/
    u = r8_uniform_01_sample ( );
    if ( d * u <= t * t * t )
    {
      return value;
    }

    r = 1.0 / a;
    q0 = (((((( 
        q7   * r 
      + q6 ) * r 
      + q5 ) * r 
      + q4 ) * r
      + q3 ) * r
      + q2 ) * r
      + q1 ) * r;
/*
  Approximation depending on size of parameter A.
*/
    if ( 13.022 < a )
    {
      b = 1.77;
      si = 0.75;
      c = 0.1515 / s;
    }
    else if ( 3.686 < a )
    {
      b = 1.654 + 0.0076 * s2;
      si = 1.68 / s + 0.275;
      c = 0.062 / s + 0.024;
    }
    else
    {
      b = 0.463 + s + 0.178 * s2;
      si = 1.235;
      c = 0.195 / s - 0.079 + 0.16 * s;
    }
/*
  Quotient test.
*/
    if ( 0.0 < x )
    {
      v = 0.5 * t / s;

      if ( 0.25 < fabs ( v ) )
      {
        q = q0 - s * t + 0.25 * t * t + 2.0 * s2 * log ( 1.0 + v );
      }
      else
      {
        q = q0 + 0.5 * t * t * (((((( 
            a7   * v 
          + a6 ) * v 
          + a5 ) * v 
          + a4 ) * v 
          + a3 ) * v 
          + a2 ) * v 
          + a1 ) * v;
      }

      if ( log ( 1.0 - u ) <= q )
      {
        return value;
      }
    }

    for ( ; ; )
    {
      e = r8_exponential_01_sample ( );
      u = 2.0 * r8_uniform_01_sample ( ) - 1.0;
 
      if ( 0.0 <= u )
      {
        t = b + fabs ( si * e );
      }
      else
      {
        t = b - fabs ( si * e );
      }
/*
  Possible rejection.
*/
      if ( t < -0.7187449 )
      {
        continue;
      }
/*
  Calculate V and quotient Q.
*/
      v = 0.5 * t / s;

      if ( 0.25 < fabs ( v ) )
      {
        q = q0 - s * t + 0.25 * t * t + 2.0 * s2 * log ( 1.0 + v );
      }
      else
      {
        q = q0 + 0.5 * t * t * (((((( 
            a7   * v 
          + a6 ) * v 
          + a5 ) * v 
          + a4 ) * v 
          + a3 ) * v 
          + a2 ) * v 
          + a1 ) * v;
      }
/*
  Hat acceptance.
*/
      if ( q <= 0.0 )
      {
        continue;
      }

      if ( 0.5 < q )
      {
        w = exp ( q ) - 1.0;
      }
      else
      {
        w = (((( 
            e5   * q 
          + e4 ) * q 
          + e3 ) * q 
          + e2 ) * q 
          + e1 ) * q;
      }
/*
  May have to sample again.
*/
      if ( c * fabs ( u ) <= w * exp ( e - 0.5 * t * t ) )
      {
        break;
      }
    }

    x = s + 0.5 * t;
    value = x * x;
  }
/*
  Method for A < 1.
*/
  else if ( a < 1.0 )
  {
    b = 1.0 + 0.3678794 * a;

    for ( ; ; )
    {
      p = b * r8_uniform_01_sample ( );

      if ( p < 1.0 )
      {
        value = exp ( log ( p ) / a );
        if ( value <= r8_exponential_01_sample ( ) )
        {
          break;
        }
      }
      else
      {
        value = - log ( ( b - p ) / a );
        if ( ( 1.0 - a ) * log ( value ) <= r8_exponential_01_sample ( ) )
        {
          break;
        }
      }
    }
  }
  return value;
}
/******************************************************************************/

double r8_invchi_pdf ( double df, double rval )
{
  double temp1;
  double temp2;
  double value; 

  if ( df <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_INVCHI_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Degrees of freedom must be positive.\n" );
    exit ( 1 );
  }
      
  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp2 = df * 0.5;

    temp1 = - temp2 * log ( 2.0 ) - ( temp2 + 1.0 ) * log ( rval ) 
      - 0.5 / rval - r8_gamma_log ( temp2 );

    value = exp ( temp1 );
  }
  return value;
}
/******************************************************************************/

double r8_invchi_sample ( double df )
{
  double a;
  double b;
  double value;

  a = 0.5;
  b = 0.5 * df;
  value = r8_gamma_sample ( a, b );

  if ( value != 0.0 )
  {
    value = 1.0 / value;
  }
  return value;
}
/******************************************************************************/

double r8_invgam_pdf ( double beta, double alpha, double rval )
{
  double temp;
  double value;

  if ( alpha <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_INVGAM_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter ALPHA is not positive.\n" );
    exit ( 1 );
  }

  if ( beta <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_INVGAM_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Parameter BETA is not positive.\n" );
    exit ( 1 );
  }

  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp = alpha * log ( beta ) - ( alpha + 1.0 ) * log ( rval ) 
      - beta / rval - r8_gamma_log ( alpha );

    value = exp ( temp );
  }

  return value;
}
/******************************************************************************/

double r8_invgam_sample ( double beta, double alpha )
{
  double value;

  value = r8_gamma_sample ( beta, alpha );
  if ( value != 0.0 )
  {
    value = 1.0 / value;
  }
  return value;
}
/******************************************************************************/

double r8_max ( double x, double y )
{
  double value;

  if ( y < x )
  {
    value = x;
  }
  else
  {
    value = y;
  }
  return value;
}
/******************************************************************************/

double r8_min ( double x, double y )
{
  double value;

  if ( y < x )
  {
    value = y;
  }
  else
  {
    value = x;
  }
  return value;
}
/******************************************************************************/

double r8_normal_pdf ( double av, double sd, double rval )
{
  double pi = 3.141592653589793;
  double rtemp;
  double value;

  if ( sd <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_NORMAL_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Standard deviation must be positive.\n" );
    exit ( 1 );
  }

  rtemp = ( rval - av ) * ( rval - av ) * 0.5 / ( sd * sd );

  value = exp ( - rtemp ) / sd / sqrt ( 2.0 * pi );

  return value;
}
/******************************************************************************/

double r8_normal_sample ( double av, double sd )
{
  double value;

  value = sd * r8_normal_01_sample ( ) + av;

  return value;
}
/******************************************************************************/

double r8_normal_01_pdf ( double rval )
{
  double pi = 3.141592653589793;
  double value;

  value = exp ( - 0.5 * rval * rval ) / sqrt ( 2.0 * pi );

  return value;
}
/******************************************************************************/

double r8_normal_01_sample ( )
{
  const double pi = 3.14159265358979323;
  double r1;
  double r2;
  double x;

  r1 = r8_uniform_01_sample ( );
  r2 = r8_uniform_01_sample ( );

  x = sqrt ( -2.0 * log ( r1 ) ) * cos ( 2.0 * pi * r2 );

  return x;
}
/******************************************************************************/

double r8_scinvchi_pdf ( double df, double s, double rval )
{
  double temp1;
  double temp2;
  double value;

  if ( df <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_SCINVCHI_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Degrees of freedom must be positive.\n" );
    exit ( 1 );
  }

  if ( s <= 0.0 )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_SCINVCHI_PDF - Fatal error!\n" );
    fprintf ( stderr, "  Scale parameter must be positive.\n" );
    exit ( 1 );
  }

  if ( rval <= 0.0 )
  {
    value = 0.0;
  }
  else
  {
    temp2 = df * 0.5;

    temp1 = temp2 * log ( temp2 ) + temp2 * log ( s ) 
      - ( temp2 * s / rval ) 
      - ( temp2 + 1.0 ) * log ( rval ) - r8_gamma_log ( temp2 );

    value = exp ( temp1 );
  }

  return value;
}
/******************************************************************************/

double r8_scinvchi_sample ( double df, double s )
{
  double a;
  double b;
  double value;

  a = 0.5 * df * s;
  b = 0.5 * df;
  value = r8_gamma_sample ( a, b );
  if ( value != 0.0 )
  {
    value = 1.0 / value;
  }
  return value;
}
/******************************************************************************/

double r8_uniform_pdf ( double lower, double upper, double rval )
{
  double value;

  if ( upper <= lower )
  {
    fprintf ( stderr, "\n" );
    fprintf ( stderr, "R8_UNIFORM_PDF - Fatal error!\n" );
    fprintf ( stderr, "  For uniform PDF, the lower limit must be\n" );
    fprintf ( stderr, "  less than the upper limit\n" );
    exit ( 1 );
  }

  if ( rval < lower )
  {
    value = 0.0;
  }
  else if ( rval <= upper )
  {
    value = 1.0 / ( upper - lower );
  }
  else
  {
    value = 0.0;
  }

  return value;
}
/******************************************************************************/

double r8_uniform_sample ( double low, double high )
{
  double value;

  value = low + ( high - low ) * r8_uniform_01_sample ( );

  return value;
}
/******************************************************************************/

double r8_uniform_01_pdf ( double rval )
{
  double value;

  if ( rval < 0.0 )
  {
    value = 0.0;
  }
  else if ( rval <= 1.0 )
  {
    value = 1.0;
  }
  else
  {
    value = 0.0;
  }

  return value;
}
/******************************************************************************/

double r8_uniform_01_sample ( )
{
  const int option = 0;
  double value;

  if ( option == 0 )
  {
    value = r8_uni_01 ( );
  }
  else
  {
    value = ( double ) rand ( ) / ( double ) RAND_MAX;
  }

  return value;
}
/******************************************************************************/

double *r8ge_mtm ( int n, double a[], double b[] )
{
  double *c;
  int i;
  int j;
  int k;

  c = ( double * ) malloc ( n * n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      c[i+j*n] = 0.0;
      for ( k = 0; k < n; k++ )
      {
        c[i+j*n] = c[i+j*n] + a[k+i*n] * b[k+j*n];
      }
    }
  }

  return c;
}
/******************************************************************************/

void r8ge_print ( int m, int n, double a[], char *title )
{
  r8ge_print_some ( m, n, a, 1, 1, m, n, title );

  return;
}
/******************************************************************************/

void r8ge_print_some ( int m, int n, double a[], int ilo, int jlo, int ihi, 
  int jhi, char *title )
{
# define INCX 5

  int i;
  int i2hi;
  int i2lo;
  int j;
  int j2hi;
  int j2lo;

  printf ( "\n" );
  printf ( "%s\n", title );
/*
  Print the columns of the matrix, in strips of 5.
*/
  for ( j2lo = jlo; j2lo <= jhi; j2lo = j2lo + INCX )
  {
    j2hi = j2lo + INCX - 1;
    j2hi = i4_min ( j2hi, n );
    j2hi = i4_min ( j2hi, jhi );

    printf ( "\n" );
/*
  For each column J in the current range...

  Write the header.
*/
    printf ( "  Col:    " );
    for ( j = j2lo; j <= j2hi; j++ )
    {
      printf ( "%7d       ", j );
    }
    printf ( "\n" );
    printf ( "  Row\n" );
    printf ( "  ---\n" );
/*
  Determine the range of the rows in this strip.
*/
    i2lo = i4_max ( ilo, 1 );
    i2hi = i4_min ( ihi, m );

    for ( i = i2lo; i <= i2hi; i++ )
    {
/*
  Print out (up to) 5 entries in row I, that lie in the current strip.
*/
      printf ( "%5d  ", i );
      for ( j = j2lo; j <= j2hi; j++ )
      {
        printf ( "%12g  ", a[i-1+(j-1)*m] );
      }
      printf ( "\n" );
    }
  }
  return;
# undef INCX
}
/******************************************************************************/

double *r8mat_mtv_new ( int m, int n, double a[], double x[] )
{
  int i;
  int j;
  double *y;

  y = ( double * ) malloc ( n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    y[j] = 0.0;
    for ( i = 0; i < m; i++ )
    {
      y[j] = y[j] + a[i+j*m] * x[i];
    }
  }

  return y;
}
/******************************************************************************/

double *r8mat_mv_new ( int m, int n, double a[], double x[] )
{
  int i;
  int j;
  double *y;

  y = ( double * ) malloc ( m * sizeof ( double ) );

  for ( i = 0; i < m; i++ )
  {
    y[i] = 0.0;
    for ( j = 0; j < n; j++ )
    {
      y[i] = y[i] + a[i+j*m] * x[j];
    }
  }

  return y;
}
/******************************************************************************/

double r8mat_norm_fro_affine ( int m, int n, double a1[], double a2[] )
{
  int i;
  int j;
  double value;

  value = 0.0;
  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      value = value + pow ( a1[i+j*m] - a2[i+j*m], 2 );
    }
  }
  value = sqrt ( value );

  return value;
}
/******************************************************************************/

double r8mat_podet ( int n, double r[] )
{
  double det;
  int i;

  det = 1.0;
  for ( i = 0; i < n; i++ )
  {
    det = det * r[i+i*n] * r[i+i*n];
  }
  return det;
}
/******************************************************************************/

double *r8mat_pofac ( int n, double a[] )
{
  double dot;
  int i;
  int j;
  int k;
  double *r;
  double s;
  double t;

  r = ( double * ) malloc ( n * n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i <= j; i++ )
    {
      r[i+j*n] = a[i+j*n];
    }
    for ( i = j + 1; i < n; i++ )
    {
      r[i+j*n] = 0.0;
    }
  }

  for ( j = 0; j < n; j++ )
  {
    s = 0.0;

    for ( k = 0; k < j; k++ )
    {
      dot = 0.0;
      for ( i = 0; i < k; i++ )
      {
        dot = dot + r[i+k*n] * r[i+j*n];
      }
      t = r[k+j*n] - dot;
      t = t / r[k+k*n];
      r[k+j*n] = t;
      s = s + t * t;
    }

    s = r[j+j*n] - s;

    if ( s < 0.0 )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "R8MAT_POFAC - Fatal error!\n" );
      fprintf ( stderr, "  The matrix is not positive definite.\n" );
      exit ( 1 );
    }

    if ( s == 0.0 )
    {
      fprintf ( stderr, "\n" );
      fprintf ( stderr, "R8MAT_POFAC - Warning!\n" );
      fprintf ( stderr, "  The matrix is not strictly positive definite.\n" );
    }

    r[j+j*n] = sqrt ( s );
  }

  return r;
}
/******************************************************************************/

double *r8mat_poinv ( int n, double r[] )
{
  double *b;
  int i;
  int j;
  int k;
  double t;

  b = ( double * ) malloc ( n * n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      b[i+j*n] = r[i+j*n];
    }
  }

  for ( k = 0; k < n; k++ )
  {
    b[k+k*n] = 1.0 / b[k+k*n];
    for ( i = 0; i < k; i++ )
    {
      b[i+k*n] = - b[i+k*n] * b[k+k*n];
    }
    for ( j = k + 1; j < n; j++ )
    {
      t = b[k+j*n];
      b[k+j*n] = 0.0;
      for ( i = 0; i <= k; i++ )
      {
        b[i+j*n] = b[i+j*n] + t * b[i+k*n];
      }
    }
  }
/*
  Form inverse(R) * (inverse(R))'.
*/
  for ( j = 0; j < n; j++ )
  {
    for ( k = 0; k < j; k++ )
    {
      t = b[k+j*n];
      for ( i = 0; i <= k; i++ )
      {
        b[i+k*n] = b[i+k*n] + t * b[i+j*n];
      }
    }
    t = b[j+j*n];
    for ( i = 0; i <= j; i++ )
    {
      b[i+j*n] = b[i+j*n] * t;
    }
  }
  return b;
}
/******************************************************************************/

double *r8mat_upsol ( int n, double r[], double b[] )
{
  int i;
  int j;
  double *x;

  x = ( double * ) malloc ( n * sizeof ( double ) );
  for ( j = 0; j < n; j++ )
  {
    x[j] = b[j];
  }

  x[n-1] = x[n-1] / r[n-1+(n-1)*n];

  for ( j = n - 2; 0 <= j; j-- )
  {
    for ( i = 0; i <= j; i++ )
    {
      x[i] = x[i] - r[i+(j+1)*n] * x[j+1];
    }
    x[j] = x[j] / r[j+j*n];
  }
  return x;
}
/******************************************************************************/

double *r8mat_utsol ( int n, double r[], double b[] )
{
  int i;
  int j;
  double *x;

  x = ( double * ) malloc ( n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    x[j] = b[j];
  }

  x[0] = x[0] / r[0+0*n];

  for ( j = 1; j < n; j++ )
  {
    for ( i = 0; i < j; i++ )
    {
      x[j] = x[j] - r[i+j*n] * x[i];
    }
    x[j] = x[j] / r[j+j*n]; 
  }
  return x;
}
/******************************************************************************/

double *r8po_fa ( int n, double a[] )
{
  double *b;
  int i;
  int j;
  int k;
  double s;

  b = ( double * ) malloc ( n * n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < n; i++ )
    {
      b[i+j*n] = a[i+j*n];
    }
  }

  for ( j = 0; j < n; j++ )
  {
    for ( k = 0; k <= j-1; k++ )
    {
      for ( i = 0; i <= k-1; i++ )
      {
        b[k+j*n] = b[k+j*n] - b[i+k*n] * b[i+j*n];
      }
      b[k+j*n] = b[k+j*n] / b[k+k*n];
    }

    s = b[j+j*n];
    for ( i = 0; i <= j-1; i++ )
    {
      s = s - b[i+j*n] * b[i+j*n];
    }

    if ( s <= 0.0 )
    {
      free ( b );
      return NULL;
    }

    b[j+j*n] = sqrt ( s );
  }
/*
  Since the Cholesky factor is in R8GE format, zero out the lower triangle.
*/
  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < i; j++ )
    {
      b[i+j*n] = 0.0;
    }
  }

  return b;
}
/******************************************************************************/

double r8vec_dot_product ( int n, double a1[], double a2[] )
{
  int i;
  double value;

  value = 0.0;
  for ( i = 0; i < n; i++ )
  {
    value = value + a1[i] * a2[i];
  }
  return value;
}
/******************************************************************************/

double r8vec_multinormal_pdf ( int n, double mu[], double r[], double c_det, 
  double x[] )
{
  double *b;
  int i;
  double pdf;
  double pi = 3.141592653589793;
  double xcx;
  double *y;
/*
  Compute:
    inverse(R')*(x-mu) = y
  by solving:
    R'*y = x-mu
*/
  b = ( double * ) malloc ( n * sizeof ( double ) );

  for ( i = 0; i < n; i++ )
  {
    b[i] = x[i] - mu[i];
  }
  y = r8mat_utsol ( n, r, b );
/*
  Compute:
    (x-mu)' * inv(C)          * (x-mu)
  = (x-mu)' * inv(R'*R)       * (x-mu)
  = (x-mu)' * inv(R) * inv(R) * (x-mu)
  = y' * y.
*/
  xcx = r8vec_dot_product ( n, y, y );

  pdf = 1.0 / sqrt ( pow ( 2.0 * pi, n ) ) 
      * 1.0 / sqrt ( c_det ) 
      * exp ( - 0.5 * xcx );

  free ( b );
  free ( y );

  return pdf;
}
/******************************************************************************/

double *r8vec_multinormal_sample ( int n, double mu[], double r[] )
{
  int i;
  int j;
  double *x;
  double *z;
/*
  Compute X = MU + R' * Z
  where Z is a vector of standard normal variates.
*/
  z = ( double * ) malloc ( n * sizeof ( double ) );
  for ( j = 0; j < n; j++ )
  {
    z[j] = r8_normal_01_sample ( );
  }

  x = ( double * ) malloc ( n * sizeof ( double ) );
  for ( i = 0; i < n; i++ )
  {
    x[i] = mu[i];
    for ( j = 0; j <= i; j++ )
    {
      x[i] = x[i] + r[j+i*n] * z[j];
    }
  }

  free ( z );

  return x;
}
