#ifndef _EXPERIMENT_ROUND_RESULT_H_
#define _EXPERIMENT_ROUND_RESULT_H_

#include "market/holdings.h"

namespace experiment {

struct RoundResult {
  int id;
  market::Holdings holdings;
  int utility;
};

}  //  namespace experiment

#endif  // _EXPERIMENT_ROUND_RESULT_H_
