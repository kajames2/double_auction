#ifndef _EXPERIMENT_DRIVER_H_
#define _EXPERIMENT_DRIVER_H_

#include <boost/asio.hpp>

namespace experiment {

class Driver {
  void StartExperiment();
  void EndRound();
  void EndExperiment();
  void Pause();
  void Resume();
  void ResetRound();
  void Reset();
};

}  // namespace experiment

#endif  // _EXPERIMENT_DRIVER_H_
