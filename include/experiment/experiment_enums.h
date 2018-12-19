#ifndef _EXPERIMENT_EXPERIMENT_ENUMS_H_
#define _EXPERIMENT_EXPERIMENT_ENUMS_H_

#include <ostream>

namespace experiment {

enum class Status { NotStarted, Running, Paused, Ended };
enum class Stage { PreExperiment, Round, Review, End };

std::ostream& operator<<(std::ostream& os, Status s);
std::ostream& operator<<(std::ostream& os, Stage s);
}  // namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_ENUMS_H_
