
// Author: Manos Papadakis

#include <RcppArmadillo.h>
#include "mn.h"
#include "Rfast.h"
#include <string>

using namespace Rcpp;
using namespace arma;
using std::string;

NumericMatrix euclidean_dist(NumericMatrix x, const bool sqr)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  if (sqr)
  {
    for (i = 0; i < ncl - 1; ++i)
    {
      xv = xx.col(i);
      for (j = i + 1; j < ncl; ++j)
      {
        a = sum(square(xx.col(j) - xv));
        f(i, j) = a;
        f(j, i) = a;
      }
    }
  }
  else
  {
    for (i = 0; i < ncl - 1; ++i)
    {
      xv = xx.col(i);
      for (j = i + 1; j < ncl; ++j)
      {
        a = std::sqrt(sum(square(xx.col(j) - xv)));
        f(i, j) = a;
        f(j, i) = a;
      }
    }
  }
  return f;
}

NumericMatrix manhattan(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(abs(xv - xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix chi_square(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(square(xv - xx.col(j)) / (xv + xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix soergel(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(abs(xv - xx.col(j))) / sum_max_elems(xv, xx.col(j));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix wave_hedges(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(abs(xv - xx.col(j)) / max_elems(xv , xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix hellinger(NumericMatrix x, const bool sqr)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  const double p = 1.0 / std::sqrt(2.0);
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;
  if (sqr)
  {
    for (i = 0; i < ncl - 1; ++i)
    {
      xv = xx.col(i);
      for (j = i + 1; j < ncl; ++j)
      {
        a = sum(square(xv - xx.col(j))) * 0.5;
        f(i, j) = a;
        f(j, i) = a;
      }
    }
  }
  else
  {
    for (i = 0; i < ncl - 1; ++i)
    {
      xv = xx.col(i);
      for (j = i + 1; j < ncl; ++j)
      {
        a = p * std::sqrt(sum(square(xv - xx.col(j))));
        f(i, j) = a;
        f(j, i) = a;
      }
    }
  }
  return f;
}

NumericMatrix max(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw), tmp(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      tmp = abs(xv - xx.col(j));
      a = tmp.at(tmp.index_max());
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix min(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw), tmp(nrw);
  double a;
  int i, j;
  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      tmp = abs(xv - xx.col(j));
      a = tmp(tmp.index_min());
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix minkowski(NumericMatrix x, const double p)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  const double p_1 = 1.0 / p;
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = pow(sum_with<std::pow, colvec>(abs(xv - xx.col(j)), p), p_1);
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix canberra(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw), absx(nrw);
  mat x_abs = abs(x);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    absx = x_abs.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(abs(xv - xx.col(j)) / (absx + x_abs.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix total_variation(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = 0.5 * sum(abs(xv - xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix sorensen(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(abs(xv - xx.col(j)) / (xv + xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

NumericMatrix cosine(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw), norm_x = euclidean_norm(xx);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    double normx = norm_x[i];
    for (j = i + 1; j < ncl; ++j)
    {
      a = dot(xv, xx.col(j)) / (normx * norm_x[j]);
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix kullback_leibler(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  NumericMatrix f(ncl, ncl), log_x(nrw, ncl);
  mat xx(x.begin(), nrw, ncl, false), log_xx(log_x.begin(), nrw, ncl, false);
  colvec xv(nrw), log_xv(nrw);
  double a;
  int i, j;

  fill_with<std::log, double *, double *>(x.begin(), x.end(), log_xx.begin());

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    log_xv = log_xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum_with_condition<double, std::isfinite, colvec>((xv - xx.col(j)) % (log_xv - log_xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix jensen_shannon(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  NumericMatrix f(ncl, ncl), log_x(nrw, ncl);
  mat xx(x.begin(), nrw, ncl, false), log_xx(log_x.begin(), nrw, ncl, false);
  colvec xv(nrw), log_xv(nrw);
  double a;
  constexpr double log2 = std::log(2);
  int i, j;
  fill_with<std::log, double *, double *>(x.begin(), x.end(), log_xx.begin());

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    log_xv = log_xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum_with_condition<double, check_if_is_finite, colvec>((xv + xx.col(j)) % (log2 - arma::log(xv + xx.col(j))) + xv % log_xv + xx.col(j) % log_xx.col(j));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix bhattacharyya(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  mat xx(x.begin(), nrw, ncl, false);
  NumericMatrix f(ncl, ncl);
  colvec xv(nrw);
  double a;
  int i, j;

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(sqrt(xv % xx.col(j)));
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix itakura_saito(NumericMatrix x)
{
  const int ncl = x.ncol(), nrw = x.nrow();
  NumericMatrix f(ncl, ncl), log_x(nrw, ncl);
  mat xx(x.begin(), nrw, ncl, false), log_xx(log_x.begin(), nrw, ncl, false);
  colvec xv(nrw), log_xv(nrw);
  double a;
  int i, j;
  fill_with<std::log, double *, double *>(x.begin(), x.end(), log_xx.begin());

  for (i = 0; i < ncl - 1; ++i)
  {
    xv = xx.col(i);
    log_xv = log_xx.col(i);
    for (j = i + 1; j < ncl; ++j)
    {
      a = sum(xv / xx.col(j) - (log_xv - log_xx.col(j)) - 1);
      f(i, j) = a;
      f(j, i) = a;
    }
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix haversine(NumericMatrix x)
{
  const int nrw = x.nrow();
  const int nrw_1 = nrw - 1;
  colvec x0(x.begin(), nrw, false), x1(x.begin() + nrw, nrw, false);
  NumericMatrix f(nrw, nrw);
  mat ff(f.begin(), nrw, nrw, false);
  colvec ind_col(nrw_1);
  colvec a(nrw_1);
  int i;

  for (i = 0; i < nrw_1; ++i)
  {
    span ind(i + 1, nrw_1);
    ind_col = x0(ind);
    a = square(sin(0.5 * (x0[i] - ind_col))) + cos(x0[i]) * (cos(ind_col) % square(sin(0.5 * (x1[i] - x1(ind)))));
    a = 2 * asin(sqrt(a));
    ff(i, ind) = a.t();
    ff(ind, i) = a;
  }
  return f;
}

//[[Rcpp::export]]
NumericMatrix dist(NumericMatrix x, const string method, const bool sqr, const int p)
{
  if (method == "euclidean" || p == 2)
  {
    return euclidean_dist(x, sqr);
  }
  else if (method == "manhattan" || p == 1)
  {
    return manhattan(x);
  }
  else if (method == "maximum")
  {
    return max(x);
  }
  else if (method == "minimum")
  {
    return min(x);
  }
  else if (method == "canberra")
  {
    return canberra(x);
  }
  else if (method == "minkowski")
  {
    return minkowski(x, p);
  }
  else if (method == "bhattacharyya")
  {
    return bhattacharyya(x);
  }
  else if (method == "hellinger")
  {
    return hellinger(x, sqr);
  }
  else if (method == "total_variation")
  {
    return total_variation(x);
  }
  else if (method == "kullback_leibler")
  {
    return kullback_leibler(x);
  }
  else if (method == "jensen_shannon")
  {
    return jensen_shannon(x);
  }
  else if (method == "itakura_saito")
  {
    return itakura_saito(x);
  }
  else if (method == "haversine")
  {
    return haversine(x);
  }
  else if (method == "chi_square")
  {
    return chi_square(x);
  }
  else if (method == "sorensen")
  {
    return sorensen(x);
  }
  else if (method == "soergel")
  {
    return soergel(x);
  }
  else if (method == "cosine")
  {
    return cosine(x);
  }
  else if (method == "wave_hedges")
  {
    return wave_hedges(x);
  }
  stop("Unsupported Method: %s", method);
}

RcppExport SEXP Rfast_dist(SEXP xSEXP, SEXP methodSEXP, SEXP sqrSEXP, SEXP pSEXP)
{
  BEGIN_RCPP
  RObject __result;
  RNGScope __rngScope;
  traits::input_parameter<NumericMatrix>::type x(xSEXP);
  traits::input_parameter<const string>::type method(methodSEXP);
  traits::input_parameter<const bool>::type sqr(sqrSEXP);
  traits::input_parameter<const int>::type p(pSEXP);
  __result = dist(x, method, sqr, p);
  return __result;
  END_RCPP
}
