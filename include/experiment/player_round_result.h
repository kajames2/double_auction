#ifndef _EXPERIMENT_PLAYER_ROUND_RESULT_H_
#define _EXPERIMENT_PLAYER_ROUND_RESULT_H_

#include "market/holdings.h"

namespace experiment {

struct PlayerRoundResult {
  int id;
  market::Holdings holdings;
  int utility;
};

}  //  namespace experiment

#endif  // _EXPERIMENT_PLAYER_ROUND_RESULT_H_
