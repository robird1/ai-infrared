//
// Created by uriah on 18-2-8.
//

#include <ctime>
#include "timeUtil.h"
namespace uls{

int64_t  uls_get_utc_time(){

  time_t t;
  t = time(NULL);
  double ti = difftime(t, 0);
  return (int64_t)ti;
}
}