#include "experiment/experiment_enums.h"

#include <ostream>

namespace experiment {

std::ostream& operator<<(std::ostream& os, Status s) {
  switch (s) {
    case Status::NotStarted:
      return os << "Not Started";
    case Status::Running:
      return os << "Running";
    case Status::Paused:
      return os << "Paused";
    case Status::Ended:
      return os << "Ended";
  }
}

std::ostream& operator<<(std::ostream& os, Stage s) {
  switch (s) {
    case Stage::PreExperiment:
      return os << "Not Started";
    case Stage::Round:
      return os << "Market Stage";
    case Stage::Review:
      return os << "Review Stage";
    case Stage::End:
      return os << "Ended";
  }
}

}  // namespace experiment
