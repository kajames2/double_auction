#ifndef _EXPERIMENT_EXPERIMENT_ENUMS_H_
#define _EXPERIMENT_EXPERIMENT_ENUMS_H_

#include <iostream>

namespace experiment {

enum class Status { NotStarted, Running, Paused, Ended };
enum class Stage { PreExperiment, Round, Review, Payouts, End };
enum class PlayerType { Buyer, Seller, Trader };

std::ostream& operator<<(std::ostream& os, Status s);
std::ostream& operator<<(std::ostream& os, Stage s);
std::ostream& operator<<(std::ostream& os, PlayerType s);
std::istream& operator>>(std::istream& is, Status& s);
std::istream& operator>>(std::istream& is, Stage& s);
std::istream& operator>>(std::istream& is, PlayerType& s);
}  // namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_ENUMS_H_
