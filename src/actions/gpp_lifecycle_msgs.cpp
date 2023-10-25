#include "action_manager.h"
#include "ros_backend.h"

#include <execution/controller.h>

#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"

template<>
ServiceManager<lifecycle_msgs::srv::ChangeState>::RequestT
ServiceManager<lifecycle_msgs::srv::ChangeState>::build_request(const gpp::Activity &a)
{
    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();

    request->transition.label = std::string(a.mapped_arg_value("transition"));

    return request;
}

void RosBackend::define_lifecycle_msgs_actions()
{
    create_ServiceManager<lifecycle_msgs::srv::ChangeState>("/pose_finder/change_state");
    create_ServiceManager<lifecycle_msgs::srv::ChangeState>("/find_object_pose/change_state");
}
