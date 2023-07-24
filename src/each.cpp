#include <RcppArmadillo.h>
#include "Rfast.h"
#include "Rfast\types.hpp"

using Rfast::Type;

using namespace arma;
using namespace Rcpp;
using namespace std;

static NumericVector eachcol_med_sum(NumericMatrix& x,NumericVector& y,SEXP ind){
  const bool is_ind_null = Rf_isNull(ind);
  const int n = is_ind_null ? x.ncol() : LENGTH(ind);
  NumericVector f(n),tmp(x.nrow());
  if(is_ind_null){
    for(int i=0;i<n;++i){
      tmp = x.column(i)+y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }else{
    IntegerVector indd(ind);
    for(int i=0;i<n;++i){
      tmp = x.column(indd[i]-1)+y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }
  return f;
}

static NumericVector eachcol_med_mult(NumericMatrix& x,NumericVector& y,SEXP ind){
  const bool is_ind_null = Rf_isNull(ind);
  const int n = is_ind_null ? x.ncol() : LENGTH(ind);
  NumericVector f(n),tmp(x.nrow());
  if(is_ind_null){
    for(int i=0;i<n;++i){
      tmp = x.column(i)*y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }else{
    IntegerVector indd(ind);
    for(int i=0;i<n;++i){
      tmp = x.column(indd[i]-1)*y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }
  return f;
}

static NumericVector eachcol_med_min(NumericMatrix& x,NumericVector& y,SEXP ind){
  const bool is_ind_null = Rf_isNull(ind);
  const int n = is_ind_null ? x.ncol() : LENGTH(ind);
  NumericVector f(n),tmp(x.nrow());
  if(is_ind_null){
    for(int i=0;i<n;++i){
      tmp = x.column(i)-y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }else{
    IntegerVector indd(ind);
    for(int i=0;i<n;++i){
      tmp = x.column(indd[i]-1)-y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }
  return f;
}

static NumericVector eachcol_med_div(NumericMatrix& x,NumericVector& y,SEXP ind){
  const bool is_ind_null = Rf_isNull(ind);
  const int n = is_ind_null ? x.ncol() : LENGTH(ind);
  NumericVector f(n),tmp(x.nrow());
  if(is_ind_null){
    for(int i=0;i<n;++i){
      tmp = x.column(i)/y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }else{
    IntegerVector indd(ind);
    for(int i=0;i<n;++i){
      tmp = x.column(indd[i]-1)/y;
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }
  return f;
}

/*template<class T=NumericVector,Mfunction<T,T,T> Func>
static NumericVector eachcol_med_helper(NumericMatrix& x,NumericVector& y,SEXP ind){
  const bool is_ind_null = Rf_isNull(ind);
  const int n = is_ind_null ? x.ncol() : LENGTH(ind);
  NumericVector f(n),tmp(x.nrow());
  if(is_ind_null){
    for(int i=0;i<n;++i){
      tmp = x.column(i);
      tmp = Func(tmp,y);
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }else{
    IntegerVector indd(ind);
    for(int i=0;i<n;++i){
      tmp = x.column(indd[i]-1);
      tmp = Func(tmp,y);
      f[i]=med_helper<NumericVector>(tmp.begin(),tmp.end());
    }
  }
  return f;
}*/


//[[Rcpp::export]]
NumericVector eachcol_apply(NumericMatrix x,NumericVector y,SEXP ind = R_NilValue,const char oper='*',const string method="sum", const bool parallel=false){
    if(method == "sum"){
        switch(oper){
        case '*': return eachcol_apply_helper<mmult<double>,madd<double> >(x,y,ind,parallel);
        case '/': return eachcol_apply_helper<mdiv<double>,madd<double> >(x,y,ind,parallel);
        case '+': return eachcol_apply_helper<madd <double>,madd<double> >(x,y,ind,parallel);
        case '-': return eachcol_apply_helper<mdiff<double>,madd<double> >(x,y,ind,parallel);
        case '^': return eachcol_apply_helper<std::pow,madd<double> >(x,y,ind,parallel);
        }
    }else if(method == "median"){
        switch(oper){
        case '*': return eachcol_med_mult(x,y,ind);
        case '/': return eachcol_med_div(x,y,ind);
        case '+': return eachcol_med_sum(x,y,ind);
        case '-': return eachcol_med_min(x,y,ind);
        case '^': stop("Error: Median does not support operator \'^\'.\n");
        }
    }else if(method == "max"){
        switch(oper){
        case '*': return eachcol_apply_helper<mmult<double>,mmax<double> >(x,y,ind,parallel);
        case '/': return eachcol_apply_helper<mdiv<double>,mmax<double> >(x,y,ind,parallel);
        case '+': return eachcol_apply_helper<madd <double>,mmax<double> >(x,y,ind,parallel);
        case '-': return eachcol_apply_helper<mdiff<double>,mmax<double> >(x,y,ind,parallel);
        case '^': return eachcol_apply_helper<std::pow,mmax<double> >(x,y,ind,parallel);
        }
    }else if(method == "min"){
        switch(oper){
        case '*': return eachcol_apply_helper<mmult<double>,mmin<double> >(x,y,ind,parallel);
        case '/': return eachcol_apply_helper<mdiv<double>,mmin<double> >(x,y,ind,parallel);
        case '+': return eachcol_apply_helper<madd <double>,mmin<double> >(x,y,ind,parallel);
        case '-': return eachcol_apply_helper<mdiff<double>,mmin<double> >(x,y,ind,parallel);
        case '^': return eachcol_apply_helper<std::pow,mmin<double> >(x,y,ind,parallel);
        }
    }
    stop("Error: wrong operation type.\n");
    return {};
}

RcppExport SEXP Rfast_eachcol_apply(SEXP xSEXP,SEXP ySEXP,SEXP ind,SEXP operSEXP,SEXP methodSEXP,SEXP parallelSEXP) {
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< NumericMatrix >::type x(xSEXP);
    traits::input_parameter< NumericVector >::type y(ySEXP);
    traits::input_parameter< const char >::type oper(operSEXP);
    traits::input_parameter< const string >::type method(methodSEXP);
    traits::input_parameter< const bool >::type parallel(parallelSEXP);
    __result = eachcol_apply(x,y,ind,oper,method,parallel);
    return __result;
END_RCPP
}



///////////////////////////////////////////////////////////////////////////////////


static SEXP eachrow(SEXP x,SEXP y,const char oper){
  switch(oper){
    case '*': 
      switch(Type::type(x)){
        case Type::Types::REAL: return eachrow_helper< double,double,mmult<double>,Type::R::Real >(x,y);
        case Type::Types::INT: return eachrow_helper< int,int,mmult<int>,Type::R::Int >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
    case '+': 
      switch(Type::type(x)){
        case Type::Types::REAL: return eachrow_helper< double,double,madd<double>,Type::R::Real >(x,y);
        case Type::Types::INT: return eachrow_helper< int,int,madd<int>,Type::R::Int >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
    case '/': 
      switch(Type::type(x)){
        case Type::Types::REAL: return eachrow_helper< double,double,mdiv<double>,Type::R::Real >(x,y);
        case Type::Types::INT: return eachrow_helper< int,int,mdiv<int>,Type::R::Int >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
    case '-': 
      switch(Type::type(x)){
        case Type::Types::REAL: return eachrow_helper< double,double,mdiff<double>,Type::R::Real >(x,y);
        case Type::Types::INT: return eachrow_helper< int,int,mdiff<int>,Type::R::Int >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      } 
    case '^': 
      switch(Type::type(x)){
        case Type::Types::REAL: return eachrow_helper< double,double,std::pow,Type::R::Real >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      } 
    case '=': 
      switch(Type::type(x)){
        case Type::Types::LOGICAL: return eachrow_helper<double,double,mequal<double>,Type::R::Lgl >(x,y);
        default: stop("Unsupported type. Type must be logical.");
      } 
      
    default: stop("The operation doesn't supported.");
  }
  return R_NilValue;
}

static double apply_eachrow(SEXP x,SEXP y,const char oper,const string method){
  if(method == "sum"){
    switch(oper){
      case '*':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mmult<double>,madd<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mmult<int>,madd<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '+':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,madd<double>,madd<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,madd<int>,madd<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '/':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiv<double>,madd<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiv<int>,madd<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '-':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiff<double>,madd<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiff<int>,madd<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '^':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,std::pow,madd<double> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      default: stop("The operation doesn't supported.");
    }
  }else if(method == "min"){
    switch(oper){
      case '*':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mmult<double>,mmin<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mmult<int>,mmin<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '+':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,madd<double>,mmin<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,madd<int>,mmin<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '/':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiv<double>,mmin<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiv<int>,mmin<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '-':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiff<double>,mmin<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiff<int>,mmin<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '^':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,std::pow,mmin<double> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      default: stop("The operation doesn't supported.");
    }
  }else if(method == "max"){
    switch(oper){
      case '*':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mmult<double>,mmax<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mmult<int>,mmax<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '+':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,madd<double>,mmax<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,madd<int>,mmax<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '/':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiv<double>,mmax<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiv<int>,mmax<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '-':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,mdiff<double>,mmax<double> >(x,y);
        case Type::Types::INT: return apply_eachrow_helper<int,mdiff<int>,mmax<int> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      case '^':  
      switch(Type::type(x)){
        case Type::Types::REAL: return apply_eachrow_helper<double,std::pow,mmax<double> >(x,y);
        default: stop("Unsupported type. Type must be numeric or integer.");
      }
      default: stop("The operation doesn't supported.");
    }
  }
  return 0.0;
}
//[[Rcpp::export]]
SEXP eachrow(SEXP x,SEXP y,const char oper,SEXP meth){
  return Rf_isNull(meth) ? eachrow(x,y,oper) : wrap(apply_eachrow(x,y,oper,as<string>(meth)));
}

RcppExport SEXP Rfast_eachrow(SEXP x,SEXP y,SEXP operSEXP,SEXP method){
BEGIN_RCPP
    RObject __result;
    RNGScope __rngScope;
    traits::input_parameter< const char  >::type oper(operSEXP);
    __result = eachrow(x,y,oper,method);
    return __result;
END_RCPP
}
