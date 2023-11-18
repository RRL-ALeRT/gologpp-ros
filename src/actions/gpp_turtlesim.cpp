#include <execution/controller.h>

#include "gologpp_agent/action_manager.h"
#include "gologpp_agent/exog_manager.h"
#include "gologpp_agent/ros_backend.h"
#include "std_msgs/msg/bool.hpp"
#include "turtlesim/action/rotate_absolute.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/kill.hpp"
#include "turtlesim/srv/spawn.hpp"

template <>
std::unordered_map<std::string, gpp::unique_ptr<gpp::Value>> ExogManager<std_msgs::msg::Bool>::params_to_map(
    const std_msgs::msg::Bool::ConstPtr& msg) {
  gpp::unique_ptr<gpp::Value> param(new gpp::Value(gpp::get_type<gpp::BoolType>(), static_cast<bool>(msg->data)));
  std::unordered_map<std::string, gpp::unique_ptr<gpp::Value>> params_to_map;
  params_to_map.insert({"data", std::move(param)});
  return params_to_map;
}

template <>
ActionManager<turtlesim::action::RotateAbsolute>::GoalT ActionManager<turtlesim::action::RotateAbsolute>::build_goal(
    const gpp::Activity& a) {
  auto goal = turtlesim::action::RotateAbsolute::Goal();
  goal.theta = a.mapped_arg_value("theta").numeric_convert<float>();
  return goal;
}

template <>
gpp::optional<gpp::Value> ActionManager<turtlesim::action::RotateAbsolute>::to_golog_constant(
    ResultT::WrappedResult result) {
  std::cout << "result" << std::endl;
  return gpp::Value(gpp::get_type<gpp::NumberType>(), result.result->delta);
}

template <>
ServiceManager<turtlesim::srv::Spawn>::RequestT ServiceManager<turtlesim::srv::Spawn>::build_request(
    const gpp::Activity& a) {
  auto request = std::make_shared<turtlesim::srv::Spawn::Request>();

  auto name = std::string(a.mapped_arg_value("name"));
  board_var_name = std::string(a.mapped_arg_value("board_var_name"));

  auto& board = GppBoard<std::shared_ptr<turtlesim::msg::Pose>>::board();
  auto pose = board[board_var_name];

  request->name = name;
  request->x = pose->x;
  request->y = pose->y;
  request->theta = pose->theta;

  return request;
}

template <>
gpp::optional<gpp::Value> ServiceManager<turtlesim::srv::Spawn>::to_golog_constant(ResponseT result) {
  return gpp::Value(gpp::get_type<gpp::StringType>(), result.get()->name);
}

template <>
ServiceManager<turtlesim::srv::Kill>::RequestT ServiceManager<turtlesim::srv::Kill>::build_request(
    const gpp::Activity& a) {
  auto request = std::make_shared<turtlesim::srv::Kill::Request>();

  request->name = std::string(a.mapped_arg_value("name"));

  return request;
}

void RosBackend::define_turtlesim_actions() {
  built_interface_names.push_back("turtlesim");

  create_ActionManager<turtlesim::action::RotateAbsolute>("/turtle1/rotate_absolute");

  create_ServiceManager<turtlesim::srv::Kill>("/kill");
  create_ServiceManager<turtlesim::srv::Spawn>("/spawn");

  create_ExogManger<std_msgs::msg::Bool>("/exog_event");
}
