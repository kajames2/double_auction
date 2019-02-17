cmake_minimum_required(VERSION 3.10.2)
project (DoubleAuction)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
 
set(SHARED_SOURCES
  ${PROJECT_SOURCE_DIR}/src/tools/pausable_timer.cc
  ${PROJECT_SOURCE_DIR}/src/market/offer_validity.cc
  ${PROJECT_SOURCE_DIR}/src/market/transaction.cc
  ${PROJECT_SOURCE_DIR}/src/market/clearing_queue.cc
  ${PROJECT_SOURCE_DIR}/src/market/market.cc
  ${PROJECT_SOURCE_DIR}/src/market/market_state.cc
  ${PROJECT_SOURCE_DIR}/src/market/ask.cc
  ${PROJECT_SOURCE_DIR}/src/market/holdings.cc
  ${PROJECT_SOURCE_DIR}/src/market/bid.cc
  ${PROJECT_SOURCE_DIR}/src/comms/message.cc
  ${PROJECT_SOURCE_DIR}/src/comms/connection_manager.cc
  ${PROJECT_SOURCE_DIR}/src/comms/connection.cc
  ${PROJECT_SOURCE_DIR}/src/comms/reconnector.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/controller_state.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/experiment.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/payout.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/experiment_state.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/experiment_enums.cc
  ${PROJECT_SOURCE_DIR}/src/experiment/controller.cc
  )

set(CLIENT_SOURCES
  ${PROJECT_SOURCE_DIR}/src/client/state.cc
  ${PROJECT_SOURCE_DIR}/src/client/command_line_outputter.cc
  )

set(SERVER_SOURCES
  ${PROJECT_SOURCE_DIR}/src/server/message_sender.cc
  ${PROJECT_SOURCE_DIR}/src/server/outputter.cc
  ${PROJECT_SOURCE_DIR}/src/server/client_message_processor.cc
  ${PROJECT_SOURCE_DIR}/src/server/logger.cc
  )

find_package( Boost REQUIRED COMPONENTS system )
include_directories( ${Boost_INCLUDE_DIRS} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(server ${PROJECT_SOURCE_DIR}/sample_experiment.cc ${SHARED_SOURCES} ${SERVER_SOURCES})
add_executable(client ${PROJECT_SOURCE_DIR}/sample_client.cc ${SHARED_SOURCES} ${CLIENT_SOURCES})

target_link_libraries(server Threads::Threads ${Boost_LIBRARIES})
target_link_libraries(client Threads::Threads ${Boost_LIBRARIES})

