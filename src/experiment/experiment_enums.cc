#include "experiment/experiment_enums.h"

#include <map>
#include <ostream>

namespace experiment {

std::string ToString(Status s) {
  switch (s) {
    case Status::NotStarted:
      return "NotStarted";
    case Status::Running:
      return "Running";
    case Status::Paused:
      return "Paused";
    case Status::Ended:
      return "Ended";
  }
}

std::string ToString(Stage s) {
  switch (s) {
    case Stage::PreExperiment:
      return "NotStarted";
    case Stage::Round:
      return "MarketStage";
    case Stage::Review:
      return "ReviewStage";
    case Stage::Payouts:
      return "PayoutsStage";
    case Stage::End:
      return "Ended";
  }
}

std::string ToString(PlayerType s) {
  switch (s) {
    case PlayerType::Buyer:
      return "Buyer";
    case PlayerType::Seller:
      return "Seller";
    case PlayerType::Trader:
      return "Trader";
  }
}

std::ostream& operator<<(std::ostream& os, Status s) {
  return os << ToString(s);
}

std::ostream& operator<<(std::ostream& os, Stage s) {
  return os << ToString(s);
}

std::ostream& operator<<(std::ostream& os, PlayerType s) {
  return os << ToString(s);
}

std::istream& operator>>(std::istream& is, Status& s) {
  static std::string NotStartedString = ToString(Status::NotStarted);
  static std::string RunningString = ToString(Status::Running);
  static std::string PausedString = ToString(Status::Paused);
  static std::string EndedString = ToString(Status::Ended);

  std::string str;
  is >> str;
  is.ignore();
  if (str == NotStartedString) {
    s = Status::NotStarted;
  } else if (str == RunningString) {
    s = Status::Running;
  } else if (str == PausedString) {
    s = Status::Paused;
  } else if (str == EndedString) {
    s = Status::Ended;
  }
  return is;
}

std::istream& operator>>(std::istream& is, Stage& s) {
  static std::string PreExperimentString = ToString(Stage::PreExperiment);
  static std::string RoundString = ToString(Stage::Round);
  static std::string ReviewString = ToString(Stage::Review);
  static std::string PayoutsString = ToString(Stage::Payouts);
  static std::string EndString = ToString(Stage::End);

  std::string str;
  is >> str;
  is.ignore();
  if (str == PreExperimentString) {
    s = Stage::PreExperiment;
  } else if (str == RoundString) {
    s = Stage::Round;
  } else if (str == ReviewString) {
    s = Stage::Review;
  } else if (str == PayoutsString) {
    s = Stage::Payouts;
  } else if (str == EndString) {
    s = Stage::End;
  }
  return is;
}

std::istream& operator>>(std::istream& is, PlayerType& s) {
  static std::string BuyerString = ToString(PlayerType::Buyer);
  static std::string SellerString = ToString(PlayerType::Seller);
  static std::string TraderString = ToString(PlayerType::Trader);

  std::string str;
  is >> str;
  is.ignore();
  if (str == BuyerString) {
    s = PlayerType::Buyer;
  } else if (str == SellerString) {
    s = PlayerType::Seller;
  } else if (str == TraderString) {
    s = PlayerType::Trader;
  }

  return is;
}

}  // namespace experiment
